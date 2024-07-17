#include "tinypb_coder.h"
#include "tinypb_protocol.h"
#include <vector>
#include "../../include/util.h"
#include "../../include/log.h"
#include <string.h>
#include <memory>
namespace kabi
{
void tinyPBCoder::encode(std::vector<abstractProtocol::s_ptr>& messages, tcpBuffer::s_ptr out_buffer)
{
    for(auto& i : messages)
    {
        std::shared_ptr<tinyPBProtocol> msg = std::dynamic_pointer_cast<tinyPBProtocol>(i);
        int len = 0;
        const char* buf = encode_tiny_pb(msg, len);
        if(buf != NULL && len != 0)
        {
            out_buffer->write_buffer(buf, len);
        }
        if(buf)
        {
            free((void*)buf);
            buf = NULL;
        }
    }
    

}
    //将buffer字节流转化为message对象
void tinyPBCoder::decode(std::vector<abstractProtocol::s_ptr>& out_messages, tcpBuffer::s_ptr buffer)
{
    while(true)
    {
        //遍历buffer， 找到PB_START以后解析出整包的长度，然后得到结束符的位置， 判断是否为PB_END
        std::vector<char> tmp = buffer->m_buffer;
        int start_index = buffer->read_index();
        int end_index = -1;
        int pb_len = 0;
        bool parse_success = false;
        int i = 0;
        for(i = start_index; i < buffer->write_index(); ++i)
        {
            if(tmp[i] == tinyPBProtocol::PB_START)
            {
                //从下取四个字节， 由于是网络字节序， 需要转为主机字节序
                if(i + 1 < buffer->write_index())
                {
                    pb_len = get_int32_from_net_byte(&tmp[i+1]);
                    DEBUGLOG("get pb_len = %d", pb_len);
                    //结束符的索引
                    int j = i + pb_len - 1;
                    if(j >= buffer->write_index())
                    {
                        continue;
                    }
                    if(tmp[j] == tinyPBProtocol::PB_END)
                    {
                        start_index = i;
                        end_index = j;
                        parse_success = true;
                        break;
                    }
                }
            }
        }
        if(i >= buffer->write_index())
        {
            //读到最后一个字节了
            DEBUGLOG("decode end, read all buffer data");
            return;
        }
        if(parse_success)
        {
            buffer->move_read_index(end_index - start_index + 1);
            std::shared_ptr<tinyPBProtocol> message = std::make_shared<tinyPBProtocol>();
            message->m_pb_len = pb_len;
            int msg_id_len_index = start_index + sizeof(char) + sizeof(message->m_pb_len);
            if(msg_id_len_index >= end_index)
            {
                message->parse_success = false;
                ERRORLOG("parse error, msg_id_len_index [%d] >= end_index [%d]", msg_id_len_index, end_index);
                continue;
            }
            message->m_msg_id_len = get_int32_from_net_byte(&tmp[msg_id_len_index]);
            DEBUGLOG("parse msg_id_index = %d", message->m_msg_id_len);

            int msg_id_index = msg_id_len_index + sizeof(message->m_msg_id_len);
            char msg_id[100] = {0};
            memcpy(&msg_id[0], &tmp[msg_id_index], message->m_msg_id_len);
            message->m_msg_id = std::string(msg_id);
            DEBUGLOG("parse msg_id  = %s", message->m_msg_id.c_str());
            int method_name_len_index = msg_id_index + message->m_msg_id_len;
            if(method_name_len_index >= end_index)
            {
                message->parse_success = false;
                ERRORLOG("parse error, method_name_len_index[%d] >= end_index[%d]", method_name_len_index, end_index);
                continue;
            }
            message->m_method_name_len = get_int32_from_net_byte(&tmp[method_name_len_index]);
            int method_name_index = method_name_len_index + sizeof(message->m_method_name_len);
            char method_name[100] = {0};
            memcpy(&method_name[0], &tmp[method_name_index], message->m_method_name_len);
            message->m_method_name = std::string(method_name);
            DEBUGLOG("parse method_name  = %s", message->m_method_name.c_str());

            int err_code_index = method_name_index + message->m_method_name_len;
            if (err_code_index >= end_index) {
                message->parse_success = false;
                ERRORLOG("parse error, err_code_index[%d] >= end_index[%d]", err_code_index, end_index);
                continue;
            }
            message->m_err_code = get_int32_from_net_byte(&tmp[err_code_index]);


            int error_info_len_index = err_code_index + sizeof(message->m_err_code);
            if (error_info_len_index >= end_index) {
                message->parse_success = false;
                ERRORLOG("parse error, error_info_len_index[%d] >= end_index[%d]", error_info_len_index, end_index);
                continue;
            }
            message->m_err_info_len = get_int32_from_net_byte(&tmp[error_info_len_index]);

            int err_info_index = error_info_len_index + sizeof(message->m_err_info_len);
            char error_info[512] = {0};
            memcpy(&error_info[0], &tmp[err_info_index], message->m_err_info_len);
            message->m_err_info = std::string(error_info);
            DEBUGLOG("parse error_info=%s", message->m_err_info.c_str());

            int pb_data_len = message->m_pb_len - message->m_method_name_len - message->m_msg_id_len - message->m_err_info_len - 2 - 24;

            int pd_data_index = err_info_index + message->m_err_info_len;
            message->m_pb_data = std::string(&tmp[pd_data_index], pb_data_len);

            // 这里校验和去解析
            message->parse_success = true;

            out_messages.push_back(message);
        }
    }
    

}
const char* tinyPBCoder::encode_tiny_pb(std::shared_ptr<tinyPBProtocol> message, int& length)
{
    if(message->m_msg_id.empty())
    {
        message->m_msg_id = "1234567";
    }
    DEBUGLOG("msg_id = %s", message->m_msg_id.c_str());
    int pb_len = 2 + 24 + message->m_msg_id.length() + message->m_method_name.length() + message->m_err_info.length() + message->m_pb_data.length();
    DEBUGLOG("pb_len = %d", pb_len);
    char* buf = reinterpret_cast<char*>(malloc(pb_len));
    char* tmp = buf; //tmp为buf指针
    *tmp = tinyPBProtocol::PB_START;
    tmp++;
    int32_t pb_len_net = htonl(pb_len);
    memcpy(tmp, &pb_len_net, sizeof(pb_len_net));
    tmp += sizeof(pb_len_net);
    int msg_id_len = message->m_msg_id.length();
    int32_t msg_id_len_net = htonl(msg_id_len);
    memcpy(tmp, &msg_id_len_net, sizeof(msg_id_len_net));
    tmp += sizeof(msg_id_len_net);
    if (!message->m_msg_id.empty()) 
    {
        memcpy(tmp, &(message->m_msg_id[0]), msg_id_len);
        tmp += msg_id_len;
    }
    int method_name_len = message->m_method_name.length();
    int32_t method_name_len_net = htonl(method_name_len);
    memcpy(tmp, &method_name_len_net, sizeof(method_name_len_net));
    tmp += sizeof(method_name_len_net);
    if (!message->m_method_name.empty()) {
        memcpy(tmp, &(message->m_method_name[0]), method_name_len);
        tmp += method_name_len;
    }

    int32_t err_code_net = htonl(message->m_err_code);
    memcpy(tmp, &err_code_net, sizeof(err_code_net));
    tmp += sizeof(err_code_net);

    int err_info_len = message->m_err_info.length();
    int32_t err_info_len_net = htonl(err_info_len);
    memcpy(tmp, &err_info_len_net, sizeof(err_info_len_net));
    tmp += sizeof(err_info_len_net);

    if (!message->m_err_info.empty()) {
        memcpy(tmp, &(message->m_err_info[0]), err_info_len);
        tmp += err_info_len;
    }

    if (!message->m_pb_data.empty()) {
        memcpy(tmp, &(message->m_pb_data[0]), message->m_pb_data.length());
        tmp += message->m_pb_data.length();
    }

    int32_t check_sum_net = htonl(1);
    memcpy(tmp, &check_sum_net, sizeof(check_sum_net));
    tmp += sizeof(check_sum_net);

    *tmp = tinyPBProtocol::PB_END;

    message->m_pb_len = pb_len;
    message->m_msg_id_len = msg_id_len;
    message->m_method_name_len = method_name_len;
    message->m_err_info_len = err_info_len;
    message->parse_success = true;
    length = pb_len;
    DEBUGLOG("encode message[%s] success", message->m_msg_id.c_str());

    return buf;
}
}