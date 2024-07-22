#ifndef KABI_KABISRC_NET_CODER_TINYPB_CODER_H
#define KABI_KABISRC_NET_CODER_TINYPB_CODER_H
#include "abstract_coder.h"
#include "tinypb_protocol.h"
namespace kabi
{
class tinyPBCoder : public abstractCoder
{
public:
    void encode(std::vector<abstractProtocol::s_ptr>& messages, tcpBuffer::s_ptr out_buffer);
    //将buffer字节流转化为message对象
    void decode(std::vector<abstractProtocol::s_ptr>& out_messages, tcpBuffer::s_ptr buffer);
    ~tinyPBCoder() {}
private:
    //把单个包转化为字节流
    const char* encode_tiny_pb(std::shared_ptr<tinyPBProtocol> message, int& length);
};
}
#endif //KABI_NET_CODER_TINYPB_CODER_H