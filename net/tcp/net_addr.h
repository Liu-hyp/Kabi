#ifndef KABI_NET_TCP_NET_ADDR_H
#define KABI_NET_TCP_NET_ADDR_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
namespace kabi
{
class netAddr
{
public:
    virtual sockaddr* get_sock_addr() = 0;
    virtual int set_family() = 0;
    virtual socklen_t get_sock_len() = 0;
    virtual std::string toString() = 0;
};
class ipNetAddr : public netAddr
{
public:
    ipNetAddr(const std::string& ip, uint16_t port);
    ipNetAddr(const std::string& addr);
    ipNetAddr(sockaddr_in addr);
    sockaddr* get_sock_addr();
    socklen_t get_sock_len();
    int set_family();
    std::string toString();
private:
    std::string m_ip;
    uint16_t m_port {0};
    sockaddr_in m_addr;
};

}
#endif //KABI_NET_TCP_NET_ADDR_H