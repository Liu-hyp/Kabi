#ifndef KABI_NET_TCP_TCP_CLIENT_H
#define KABI_NET_TCP_TCP_CLIENT_H
#include "net_addr.h"
#include "../eventloop.h"
#include "tcp_connection.h"
#include "../coder/abstract_protocol.h"
#include <functional>
namespace kabi
{
class tcpClient
{
public:
    tcpClient(netAddr::s_ptr peer_addr);
    ~tcpClient();
    //异步的进行
    //如果connect成功，done会被执行
    void tcp_connect(std::function<void()> done);
    //异步发送message
    //如果发送message成功， 会调用done函数，函数的入参就是message对象
    void write_msg(abstractProtocol::s_ptr message, std::function<void(abstractProtocol::s_ptr)> done);
    void read_msg(const std::string& msg_id, std::function<void(abstractProtocol::s_ptr)> done);
private:
    netAddr::s_ptr m_peer_addr;
    eventloop* m_event_loop {NULL};

    int m_fd {-1};
    fdEvent* m_fd_event {NULL};
    tcpConnection::s_ptr m_connection;
};
}
#endif