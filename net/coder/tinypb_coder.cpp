#include "tinypb_coder.h"
#include "tinypb_protocol.h"
#include <vector>
#include "../../include/util.h"
#include "../../include/log.h"
#include <string.h>
namespace kabi
{
void tinyPBCoder::encode(std::vector<abstractProtocol::s_ptr>& messages, tcpBuffer::s_ptr out_buffer)
{

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
            int req_id_len_index = start_index + sizeof(char) + sizeof(message->m_pb_len);
            if(req_id_len_index >= end_index)
            {
                message->parse_success = false;
                ERRORLOG("parse error, req_id_len_index [%d] >= end_index [%d]", req_id_len_index, end_index);
            }
            message->m_req_id_len = get_int32_from_net_byte(&tmp[req_id_len_index]);
            DEBUGLOG("parse req_id_index = %d", message->m_req_id_len);

            int req_id_index = req_id_len_index + sizeof(message->m_req_id_len);
            char req_id[100] = {0};
            memcpy(&req_id[0], &tmp[req_id_index], message->m_req_id_len);
            message->m_req_id = std::string(req_id);
            DEBUGLOG("parse req_id  = %s", message->m_req_id.c_str());
            int method_name_len_index = req_id_index + message->m_req_id_len;
            if(method_name_len_index >= end_index)
            {
                message->parse_success = false;
                ERRORLOG("parse error, method_name_len_index[%d] >= end_index[%d]", method_name_len_index, end_index);
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
            }
            message->m_err_code = get_int32_from_net_byte(&tmp[err_code_index]);


            int error_info_len_index = err_code_index + sizeof(message->m_err_code);
            if (error_info_len_index >= end_index) {
                message->parse_success = false;
                ERRORLOG("parse error, error_info_len_index[%d] >= end_index[%d]", error_info_len_index, end_index);
            }
            message->m_err_info_len = get_int32_from_net_byte(&tmp[error_info_len_index]);

            int err_info_index = error_info_len_index + sizeof(message->m_err_info_len);
            char error_info[512] = {0};
            memcpy(&error_info[0], &tmp[err_info_index], message->m_err_info_len);
            message->m_err_info = std::string(error_info);
            DEBUGLOG("parse error_info=%s", message->m_err_info.c_str());

            int pb_data_len = message->m_pb_len - message->m_method_name_len - message->m_req_id_len - message->m_err_info_len - 2 - 24;

            int pd_data_index = err_info_index + message->m_err_info_len;
            message->m_pb_data = std::string(&tmp[pd_data_index], pb_data_len);

            // 这里校验和去解析
            message->parse_success = true;

            out_messages.push_back(message);
        }
    }

}
}