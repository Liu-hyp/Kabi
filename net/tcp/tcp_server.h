#ifndef KABI_NET_TCP_TCP_SERVER_H
#define KABI_NET_TCP_TCP_SERVER_H
#include "tcp_acceptor.h"
#include "net_addr.h"
#include "../eventloop.h"
#include "../io_thread_group.h"
namespace kabi
{
class tcpServer
{
public:
    tcpServer(netAddr::s_ptr local_addr);
    ~tcpServer();
    void start();
private:
    void init();
    //当有新客户端连接之后，需要执行
    void on_accept();
private:
    tcpAcceptor::s_ptr m_acceptor;
    netAddr::s_ptr m_local_addr; //本地监听地址
    eventloop* m_main_event_loop {NULL}; // mainReactor
    ioThreadGroup* m_io_thread_group {NULL}; // subReactor
    fdEvent* m_listen_fd_event {NULL};
    int m_client_counts {0};
};
}
#endif