#include "net_addr.h"
#include <string.h>
#include "../../include/log.h"
namespace kabi
{
ipNetAddr::ipNetAddr(const std::string& ip, uint16_t port): m_ip(ip), m_port(port)
{
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    m_addr.sin_port = htons(m_port);

}
ipNetAddr::ipNetAddr(const std::string& addr)
{
    size_t i = addr.find_first_of(":"); //分割ip地址和端口号
    if( i == addr.npos)
    {
        ERRORLOG("invalid ipv4 addr %s", addr.c_str());
        return;
    }
    m_ip = addr.substr(0, i);
    m_port = std::atoi(addr.substr(i + 1, addr.size() - i - 1).c_str());
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    m_addr.sin_port = htons(m_port);

}
ipNetAddr::ipNetAddr(sockaddr_in addr):m_addr(addr)
{
    m_ip = std::string(inet_ntoa(m_addr.sin_addr));
    m_port = ntohs(m_addr.sin_port);
}
sockaddr* ipNetAddr::get_sock_addr()
{
    return reinterpret_cast<sockaddr*>(&m_addr);
}
socklen_t ipNetAddr::get_sock_len()
{
    return sizeof(m_addr);
}
int ipNetAddr::get_family()
{
    return AF_INET;
}
std::string ipNetAddr::toString()
{
    std::string re;
    re = m_ip + ":" + std::to_string(m_port);
    return re;
}
bool ipNetAddr::check_valid()
{
    if(m_ip.empty())
    {
        return false;
    }
    if(m_port <= 0 || m_port > 65536)
    {
        return false;
    }
    if(inet_addr(m_ip.c_str()) == INADDR_NONE)
    {
        return false;
    }
    return true;
}
}