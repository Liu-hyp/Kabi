#include "tcp_acceptor.h"
#include "../../include/log.h"
#include <sys/socket.h>
#include <fcntl.h>
namespace kabi
{
tcpAcceptor::tcpAcceptor(netAddr::s_ptr local_addr) : m_local_addr(local_addr)
{
    if(!local_addr->check_valid())
    {
        ERRORLOG("invalid local addr %s", local_addr->toString().c_str());
        exit(0);
    }
    m_family = m_local_addr->get_family();
    m_listenfd = socket(m_family, SOCK_STREAM, 0);
    if(m_listenfd < 0)
    {
        ERRORLOG("invalid listenfd %s", local_addr->toString().c_str());
        exit(0);
    }
    int val = 1;
    if(setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) != 0) //防止timewait占用端口，达到服务器的快速启动
    {
        ERRORLOG("set socket REUSEADDR error, errno = %d, error = %s", errno, strerror(errno));
    }
    socklen_t len = m_local_addr->get_sock_len();
    if(bind(m_listenfd, m_local_addr->get_sock_addr(), len) != 0)
    {
        ERRORLOG("bind error, errno = %d, error = %s", errno, strerror(errno));
        exit(0);
    }
    if(listen(m_listenfd, 1000) != 0)
    {
        ERRORLOG("listen error, errno = %d, error = %s", errno, strerror(errno));
        exit(0);
    }

}
tcpAcceptor::~tcpAcceptor()
{

}
int tcpAcceptor::tcp_accept()
{
    if(m_family == AF_INET)
    {
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = ::accept(m_listenfd, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);
        if(client_fd < 0)
        {
            ERRORLOG("accept error, errno = %d, error = %s", errno, strerror(errno));
        }
        ipNetAddr peer_addr(client_addr);
        INFOLOG("A client have accepted succ, peer addr [%s]", peer_addr.toString());
        return client_fd;
    }
    else
    {
        //...其他协议可以扩展
    }
}
int tcpAcceptor::get_listen_fd()
{
    return m_listenfd;
}
}