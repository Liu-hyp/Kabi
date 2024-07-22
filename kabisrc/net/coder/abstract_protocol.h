#ifndef KABI_KABISRC_NET_ABSTRACT_PROTOCOL_H
#define KABI_KABISRC_NET_ABSTRACT_PROTOCOL_H
#include <memory>
#include <string>
#include "../tcp/tcpbuffer.h"
namespace kabi
{
struct abstractProtocol : public std::enable_shared_from_this<abstractProtocol>
{
public:
    typedef std::shared_ptr<abstractProtocol>s_ptr;   
    virtual ~abstractProtocol(){}
    
public:
    std::string m_msg_id; //请求号，唯一标识一个请求和响应
};
}
#endif //KABI_NET_ABSTRACT_PROTOCOL_H