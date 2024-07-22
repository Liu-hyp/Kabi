#ifndef KABI_KABISRC_NET_TCP_TCP_ACCEPTOR_H
#define KABI_KABISRC_NET_TCP_TCP_ACCEPTOR_H
#include "net_addr.h"
#include <memory>
namespace kabi
{
class tcpAcceptor
{
public:
    typedef std::shared_ptr<tcpAcceptor> s_ptr;
    tcpAcceptor(netAddr::s_ptr local_addr);
    ~tcpAcceptor();
    std::pair<int, netAddr::s_ptr> tcp_accept();
    int get_listen_fd();
private:
    netAddr::s_ptr m_local_addr;//addr ip:port
    int m_family {-1};
    int m_listenfd {-1};//listenfd

};
}
#endif //KABI_NET_TCP_TCP_ACCEPTOR_H