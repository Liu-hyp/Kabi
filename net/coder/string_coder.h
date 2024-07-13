#ifndef KABI_NET_STRING_CODER_H
#define KABI_NET_STRING_CODER_H
#include "abstract_protocol.h"
#include "abstract_coder.h"
namespace kabi
{
class stringProtocol : public abstractProtocol {
public:
  std::string info;
};
class stringCoder : public abstractCoder
{
    // 将 message 对象转化为字节流，写入到 buffer
  void encode(std::vector<abstractProtocol::s_ptr>& messages, tcpBuffer::s_ptr out_buffer) {
    for (size_t i = 0; i < messages.size(); ++i) {
      std::shared_ptr<stringProtocol> msg = std::dynamic_pointer_cast<stringProtocol>(messages[i]);
      out_buffer->write_buffer(msg->info.c_str(), msg->info.length());
    }
  }

  // 将 buffer 里面的字节流转换为 message 对象
  void decode(std::vector<abstractProtocol::s_ptr>& out_messages, tcpBuffer::s_ptr buffer) {
    std::vector<char> re;
    buffer->read_from_buffer(re, buffer->read_able());
    std::string info;
    for (size_t i = 0; i < re.size(); ++i) {
      info += re[i];
    }

    std::shared_ptr<stringProtocol> msg = std::make_shared<stringProtocol>();
    msg->info = info;
    msg->m_req_id = "123456";
    out_messages.push_back(msg);
  }
};
}
#endif //KABI_NET_STRING_CODER_H