#ifndef KABI_KABISRC_NET_ABSTRACT_CODER_H
#define KABI_KABISRC_NET_ABSTRACT_CODER_H
#include "../tcp/tcpbuffer.h"
#include "abstract_protocol.h"
#include <vector>
#include <memory>
namespace kabi
{
class abstractCoder
{
public:
    typedef std::shared_ptr<abstractCoder>s_ptr;
    //将message对象转化为字节流，写入到buffer
    virtual void encode(std::vector<abstractProtocol::s_ptr>& messages, tcpBuffer::s_ptr out_buffer) = 0;
    //将buffer字节流转化为message对象
    virtual void decode(std::vector<abstractProtocol::s_ptr>& out_messages, tcpBuffer::s_ptr buffer) = 0;
    virtual ~abstractCoder() {}
};
}
#endif