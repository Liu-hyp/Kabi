#ifndef KABI_NET_CODER_TINYPB_PROTOCOL_H
#define KABI_NET_CODER_TINYPB_PROTOCOL_H
#include "abstract_protocol.h"
#include <string>
namespace kabi
{
struct tinyPBProtocol : public abstractProtocol
{
public:
    static char PB_START;
    static char PB_END;
public:
    int32_t m_pb_len {0};
    int32_t m_req_id_len {0};
    //req_id在父类中已经有了
    int32_t m_method_len {0};
    std::string m_method_name;
    int32_t m_err_code {0};
    int32_t m_err_info_len {0};
    std::string m_err_info;
    std::string m_pb_data;
    int32_t m_check_sum {0};

};
char tinyPBProtocol::PB_START = 0x02;
char tinyPBProtocol::PB_END = 0x03;
}
#endif