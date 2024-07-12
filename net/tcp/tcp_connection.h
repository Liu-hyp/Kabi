#ifndef KABI_NET_TCP_TCP_CONNECTION_H
#define KABI_NET_TCP_TCP_CONNECTION_H
#include "net_addr.h"
#include "tcpbuffer.h"
#include "../io_thread.h"
#include <memory>
namespace kabi
{
enum class TCPSTATE
{
    NOTCONNECTED = 1,
    CONNECTED = 2,
    HALFCONNECTED = 3,
    CLOSED = 4,
};
enum class TCPCONNECTIONTYPE
{
    SERVER = 1, //作为服务端使用，代表跟对端客户端连接
    CLIENT = 2, //作为客户端使用，代表跟对端服务端连接
};
class tcpConnection
{

public:
    typedef std::shared_ptr<tcpConnection> s_ptr;   
public:
    tcpConnection(eventloop* event_loop, int fd, int buffer_size, netAddr::s_ptr peer_addr);
    ~tcpConnection();
    void on_read();
    void excute();
    void on_write();
    void set_state(const TCPSTATE state);
    TCPSTATE get_state();
    void clear();
    void shut_down(); //服务器主动关闭连接
    void set_connection_type(TCPCONNECTIONTYPE type);
private:
    netAddr::s_ptr m_local_addr;
    netAddr::s_ptr m_peer_addr;
    tcpBuffer::s_ptr m_in_buffer; //接受缓冲区
    tcpBuffer::s_ptr m_out_buffer; //发送缓冲区
    eventloop* m_event_loop {NULL}; //代表持有该连接的io线程
    fdEvent* m_fd_event {NULL}; 
    TCPSTATE m_state;
    int m_fd {0};
    TCPCONNECTIONTYPE m_connection_type {TCPCONNECTIONTYPE::SERVER};
};
}
#endif //KABI_NET_TCP_TCP_CONNECTION_H