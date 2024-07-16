#include "tcp_client.h"
#include "../../include/log.h"
#include "../eventloop.h"
#include "../fd_event_group.h"
#include "unistd.h"
#include <string.h>
namespace kabi{
tcpClient::tcpClient(netAddr::s_ptr peer_addr) : m_peer_addr(peer_addr)
{
    m_event_loop = eventloop::get_current_event_loop();
    m_fd = socket(peer_addr->get_family(), SOCK_STREAM, 0);
    if(m_fd < 0)
    {
        ERRORLOG("tcpClient::tcpclient() error, failed to create fd");
        return;
    }
    m_fd_event = fdEventGroup::get_fd_event_group()->get_fd_event(m_fd);
    m_fd_event->set_nonblock();
    m_connection = std::make_shared<tcpConnection>(m_event_loop, m_fd, 128, m_peer_addr, nullptr, TCPCONNECTIONTYPE::CLIENT);
    m_connection->set_connection_type(TCPCONNECTIONTYPE::CLIENT);
}
tcpClient::~tcpClient()
{
    if(m_fd > 0)
    {
        close(m_fd);
    }
}
//异步的进行
//如果connect成功，done会被执行
void tcpClient::tcp_connect(std::function<void()> done)
{
    int rt = ::connect(m_fd, m_peer_addr->get_sock_addr(), m_peer_addr->get_sock_len());
    if(rt == 0)
    {
        DEBUGLOG("connect success");
        if(done)
        {
            done();
        }
    }else if(rt == -1)
    {
        if(errno == EINPROGRESS)
        {
            //epoll监听可写事件，然后判断错误码
            m_fd_event->listen(fdEvent::FdTriggerEvent::OUT_EVENT, [this, done](){
                int error = -1;
                socklen_t error_len = sizeof(error);
                getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &error, &error_len);
                bool is_connect_succ = false;
                if(error == 0)
                {
                    DEBUGLOG("connect [%s] success", m_peer_addr->toString().c_str());
                    is_connect_succ = true;
                    m_connection->set_state(TCPSTATE::CONNECTED);
                                      
                }else
                {
                    ERRORLOG("connect error, errno = %d, error = %s", errno, strerror(errno));
                }
                //需要取出可写事件的监听，不然会一直触发可写事件
                m_fd_event->cancel(fdEvent::FdTriggerEvent::OUT_EVENT);
                m_event_loop->add_epoll_event(m_fd_event);
                //如果连接成功，才会执行回调函数
                if(is_connect_succ && done)
                {                    
                    done();                   
                } 
            });
            m_event_loop->add_epoll_event(m_fd_event);
            if(!m_event_loop->is_looping())
            {
                m_event_loop->loop();
            }
        }
        else 
        {
            ERRORLOG("connect error, errno = %d, error = %s", errno, strerror(errno));
        }        
        
    }

}
//异步发送message
//如果发送message成功， 会调用done函数，函数的入参就是message对象
void tcpClient::write_msg(abstractProtocol::s_ptr message, std::function<void(abstractProtocol::s_ptr)> done)
{
    //1.把message对象写入Connection的buffer， done也要写入
    //2.启动connection可写事件
    m_connection->push_send_msg(message, done);
    m_connection->listen_write_event();
}
void tcpClient::read_msg(const std::string& req_id, std::function<void(abstractProtocol::s_ptr)> done)
{
    //1.监听可读事件
    //2.从buffer里decode得到message对象，判断是否req_id相等，相等则执行回调
    m_connection->push_read_msg(req_id, done);
    m_connection->listen_read_event();
}

}