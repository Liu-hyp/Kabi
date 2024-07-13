#ifndef KABI_NET_ABSTRACT_PROTOCOL_H
#define KABI_NET_ABSTRACT_PROTOCOL_H
#include <memory>
#include <string>
#include "tcp/tcpbuffer.h"
namespace kabi
{
class abstractProtocol : public std::enable_shared_from_this<abstractProtocol>
{
public:
    typedef std::shared_ptr<abstractProtocol>s_ptr;
    std::string get_reqId()
    {
        return m_req_id;
    }
    void set_reqId(const std::string reqId)
    {
        m_req_id = reqId;
    }
    virtual ~abstractProtocol(){}
    
protected:
    std::string m_req_id; //请求号，唯一标识一个请求和响应
private:


};
}
#endif //KABI_NET_ABSTRACT_PROTOCOL_H