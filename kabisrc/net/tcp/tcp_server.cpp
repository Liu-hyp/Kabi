#include "tcp_server.h"
#include "../../include/log.h"
#include "tcp_connection.h"
#include "../../include/config.h"
namespace kabi
{
tcpServer::tcpServer(netAddr::s_ptr local_addr) : m_local_addr(local_addr)
{
    init();
    INFOLOG("kabi tcpServer listen success on [%s]", m_local_addr->toString().c_str());
}
tcpServer::~tcpServer()
{
    if(m_main_event_loop)
    {
        delete m_main_event_loop;
        m_main_event_loop = nullptr;
    }
}
void tcpServer::start()
{
    m_io_thread_group->start();
    m_main_event_loop->loop();
}
void tcpServer::init()
{
    //tcpServer是全局的单例对象，只能由主线程去构建这个对象
    m_acceptor = std::make_shared<tcpAcceptor>(m_local_addr);
    m_main_event_loop = eventloop::get_current_event_loop();
    m_io_thread_group = new ioThreadGroup(config::get_global_config()->m_io_threads);
    m_listen_fd_event = new fdEvent(m_acceptor->get_listen_fd());
    m_listen_fd_event->listen(fdEvent::FdTriggerEvent::IN_EVENT, std::bind(&tcpServer::on_accept, this));
    m_main_event_loop->add_epoll_event(m_listen_fd_event);   
}

void tcpServer::on_accept()
{
    auto re = m_acceptor->tcp_accept();
    int client_fd = re.first;
    netAddr::s_ptr peer_addr = re.second;
    fdEvent client_fd_event(client_fd);
    m_client_counts++;
    //TODO::把clientfd添加到任意IO线程里面
    //m_io_thread_group->get_io_thread()->get_event_loop()->add_epoll_event(client_fd_event);
    ioThread* io_thread = m_io_thread_group->get_io_thread();
    tcpConnection::s_ptr connection = std::make_shared<tcpConnection>(io_thread->get_event_loop(), client_fd, 128, peer_addr, m_local_addr);
    connection->set_state(TCPSTATE::CONNECTED);
    m_client.insert(connection);
    INFOLOG("tcpServer success get client, fd = %d", client_fd);
}
}