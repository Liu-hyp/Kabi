#include "tcp_connection.h"
#include "../fd_event_group.h"
#include "../../include/log.h"
#include <unistd.h>
namespace kabi
{
tcpConnection::tcpConnection(eventloop* event_loop, int fd, int buffer_size, netAddr::s_ptr peer_addr)
:m_event_loop(event_loop), m_peer_addr(peer_addr), m_state(TCPSTATE::NOTCONNECTED), m_fd(fd)
{
    m_in_buffer = std::make_shared<tcpBuffer>(buffer_size);
    m_out_buffer = std::make_shared<tcpBuffer>(buffer_size);
    m_fd_event = fdEventGroup::get_fd_event_group()->get_fd_event(fd);
    m_fd_event->set_nonblock();
    m_fd_event->listen(fdEvent::FdTriggerEvent::IN_EVENT, std::bind(&tcpConnection::on_read, this));
    event_loop->add_epoll_event(m_fd_event);
}
tcpConnection::~tcpConnection()
{
    DEBUGLOG("~tcpConnection");
}
void tcpConnection::on_read()
{
    //1.从socket缓冲区，调用系统的read函数读取字节 inbuffer 里面
    if(m_state != TCPSTATE::CONNECTED)
    {
        INFOLOG("Onread error, client has already disconnected, addr[%s], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd);
        return;
    }
    bool is_read_all = false;
    bool is_close = false;
    while(!is_read_all)
    {
        if(m_in_buffer->write_able() == 0)
        {
            m_in_buffer->resize_buffer(2 * m_in_buffer->m_buffer.size());
        }
        int read_count = m_in_buffer->write_able();
        int write_index = m_in_buffer->write_index();
        int rt = read(m_fd, &(m_in_buffer->m_buffer[write_index]), read_count);
        std::string msg;
        for(int i = 0; i < m_in_buffer->m_buffer.size(); ++i)
        {
            msg += m_in_buffer->m_buffer[i];
        }
        DEBUGLOG("success read %d bytes from addr[%s], client fd[%d], msg = %s", rt, m_peer_addr->toString().c_str(), m_fd, msg.c_str());
        if(rt > 0)
        {
            m_in_buffer->move_write_index(rt);
            if(rt == read_count)
            {
                continue;
            }
            else if(rt < read_count)
            {
                is_read_all = true;
                break;
            }
        }
        else if(rt == 0)
        {
            //当发生了可读事件但是读不到数据，说明对方关闭了连接
            is_close = true;
            break;
        }
        else if(rt == -1 && errno == EAGAIN)
        {
            is_read_all = true;
            //非阻塞读不到数据后就会报EAGAIN这个错
            break;
        }
    }
    if(is_close)
    {
        //处理关闭连接   
        DEBUGLOG("peer closed, peer addr[%s], client fd[%d]", m_peer_addr->toString().c_str(), m_fd);
        clear();
        return;
    }
    if(!is_read_all)
    {
        ERRORLOG("not read all data");
    }
    
    // TODO::简单的echo，后面补充rpc协议的解析
    excute();
}
void tcpConnection::excute()
{
    //将rpc请求执行业务逻辑， 获取rpc响应， 再把rpc响应发送回去
    std::vector<char>tmp;
    int size = m_in_buffer->read_able();
    m_in_buffer->read_from_buffer(tmp, size);
    std::string msg;
    for(int i = 0; i < tmp.size(); ++i)
    {
        msg += tmp[i];
    }
    INFOLOG("success get request[%s] from client[%s]", msg.c_str(), m_peer_addr->toString().c_str());
    m_out_buffer->write_buffer(msg.c_str(), msg.length());
    m_fd_event->listen(fdEvent::FdTriggerEvent::OUT_EVENT, std::bind(&tcpConnection::on_write, this));
    m_event_loop->add_epoll_event(m_fd_event);
}
void tcpConnection::on_write()
{
    //将当前 out_buffer 里面的数据全部发送给client
    if(m_state != TCPSTATE::CONNECTED)
    {
        INFOLOG("Onwrite error, client has already disconnected, addr[%s], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd);
        return;
    }
    bool is_write_all = false;
    while(true)
    {
        if(m_out_buffer->read_able() == 0)
        {
            DEBUGLOG("no data need to send to client [%s]", m_peer_addr->toString().c_str());
            is_write_all = true;
            break;
        }
        int write_size = m_out_buffer->read_able();
        int read_index = m_out_buffer->read_index();
        int rt = write(m_fd, &(m_out_buffer->m_buffer[read_index]), write_size);
        if(rt >= write_size)
        {
            DEBUGLOG("no data need to send to client [%s]", m_peer_addr->toString().c_str());
            is_write_all = true;
            break;
        } else if(rt == -1 && errno == EAGAIN)
        {
            //发送缓冲区已满，不能再发送
            //这种情况直接等下次 fd 可写的时候再次发送数据即可
            ERRORLOG("write data error, errno == EAGAIN and rt == -1");
            break;
        }
    }
    if(is_write_all)
    {
        m_fd_event->cancel(fdEvent::FdTriggerEvent::OUT_EVENT);
        m_event_loop->add_epoll_event(m_fd_event);
    }
}
void tcpConnection::set_state(const TCPSTATE state)
{
    m_state = TCPSTATE::CONNECTED;
}
TCPSTATE tcpConnection::get_state()
{
    return m_state;
}
void tcpConnection::clear()
{
    //处理一些关闭连接后的清理动作
    if(m_state == TCPSTATE::CLOSED)
    {
        return;
    }
    m_fd_event->cancel(fdEvent::FdTriggerEvent::IN_EVENT);
    m_fd_event->cancel(fdEvent::FdTriggerEvent::OUT_EVENT);
    m_event_loop->del_epoll_event(m_fd_event);
    m_state = TCPSTATE::CLOSED;
}
void tcpConnection::shut_down()
{
    if(m_state == TCPSTATE::CLOSED || m_state == TCPSTATE::NOTCONNECTED)
    {
        return;
    }
    //处于半关闭
    m_state = TCPSTATE::HALFCONNECTED;
    //调用shutdown关闭读写， 意味着服务器不会再对这个fd进行读写操作了
    //触发一个FIN报文，触发四次挥手的第一个阶段
    //当fd发生可读事件但是可读的数据为0时，表示对端也没有数据发送了，对端发送了一个FIN报文，服务器就会进入time_wait状态
    ::shutdown(m_fd, SHUT_RDWR);
}
void tcpConnection::set_connection_type(TCPCONNECTIONTYPE type)
{
    m_connection_type = type;
}

}