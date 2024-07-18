#include "rpc_dispatcher.h"
#include "../coder/tinypb_protocol.h"
#include "../../include/log.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "../../include/error_code.h"
#include "rpc_controller.h"
#include "../tcp/net_addr.h"
#include "../tcp/tcp_connection.h"
namespace kabi
{
static rpcDispatcher* g_rpc_dispatcher = NULL;
rpcDispatcher* rpcDispatcher::get_rpc_dispatcher()
{
    if(g_rpc_dispatcher != NULL)
    {
        return g_rpc_dispatcher;
    }
    g_rpc_dispatcher = new rpcDispatcher();
    return g_rpc_dispatcher;
}
void rpcDispatcher::dispatch(abstractProtocol::s_ptr request, abstractProtocol::s_ptr response, tcpConnection* connection)
{
    std::shared_ptr<tinyPBProtocol> req_protocol = std::dynamic_pointer_cast<tinyPBProtocol>(request);
    std::shared_ptr<tinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<tinyPBProtocol>(response);
    std::string method_full_name = req_protocol->m_method_name;
    std::string service_name;
    std::string method_name;

    rsp_protocol->m_msg_id = req_protocol->m_msg_id;
    rsp_protocol->m_method_name = req_protocol->m_method_name;
    if(!parse_service_full_name(method_full_name, service_name, method_name))
    {
        //TODO::出错处理，解析方法名错误
        set_tinyPB_error(rsp_protocol, ERROR_PARSE_SERVICE_NAME, "parse service name error");
        return;
    }
    auto it = m_service_map.find(service_name);
    if(it == m_service_map.end())
    {
        //TODO::出错处理
        ERRORLOG("[%s] | service name[%s] not found", req_protocol->m_msg_id.c_str(), service_name.c_str());
        set_tinyPB_error(rsp_protocol, ERROR_SERVICE_NOT_FOUND, "service not found");
        return;
    }
    service_s_ptr service = (*it).second;
    const google::protobuf::MethodDescriptor* method = service->GetDescriptor()->FindMethodByName(method_name);
    if(method == NULL)
    {
        //TODO::
        ERRORLOG("[%s] | method name[%s] not found in service[%s]", req_protocol->m_msg_id.c_str(), method_name.c_str(), service_name.c_str());
        set_tinyPB_error(rsp_protocol, ERROR_METHOD_NOT_FOUND, "method not found");
        return;
    }
    google::protobuf::Message* req_msg = service->GetRequestPrototype(method).New();
    //反序列化，将pb_data反序列化req_msg
    if(!req_msg->ParseFromString(req_protocol->m_pb_data))
    {
        //TODO::失败处理
        ERRORLOG("[%s] | deserialize error", req_protocol->m_msg_id.c_str());
        set_tinyPB_error(rsp_protocol, ERROR_FAILED_DESERIALIZE, "deserialize error");
        if(req_msg != NULL)
        {
            delete req_msg;
            req_msg = nullptr;
        }
        return;
    }
    INFOLOG("[%s] | get rpc request[%s]", req_protocol->m_msg_id.c_str(), req_msg->ShortDebugString().c_str());
    google::protobuf::Message* rsp_msg = service->GetResponsePrototype(method).New();

    rpcController rpc_controller;
    rpc_controller.SetLocalAddr(connection->get_local_addr());
    rpc_controller.SetPeerAddr(connection->get_peer_addr());
    rpc_controller.SetMsgId(req_protocol->m_msg_id);

    service->CallMethod(method, &rpc_controller, req_msg, rsp_msg, NULL);   
    if(!rsp_msg->SerializeToString(&(rsp_protocol->m_pb_data)))
    {
        ERRORLOG("[%s] | serialize error, origin message [%s]", req_protocol->m_msg_id.c_str(), rsp_msg->ShortDebugString().c_str());
        set_tinyPB_error(rsp_protocol, ERROR_FAILED_SERIALIZE, "serialize error");
        if(req_msg != NULL)
        {
            delete req_msg;
            req_msg = nullptr;
        }
        if(rsp_msg != NULL)
        {
            delete rsp_msg;
            rsp_msg = nullptr;
        }
        return;
    }
    rsp_protocol->m_err_code = 0;
    INFOLOG("[%s] | dispatch success, request[%s], response[%s]",rsp_protocol->m_msg_id.c_str(), req_msg->ShortDebugString().c_str(), rsp_msg->ShortDebugString().c_str());
    if(req_msg != NULL)
    {
        delete req_msg;
        req_msg = nullptr;
    }
    if(rsp_msg != NULL)
    {
        delete rsp_msg;
        rsp_msg = nullptr;
    }
}
void rpcDispatcher::register_service(service_s_ptr service)
{
    std::string service_name = service->GetDescriptor()->full_name();
    m_service_map[service_name] = service;


}
bool rpcDispatcher::parse_service_full_name(const std::string& full_name, std::string& service_name, std::string& method_name)
{
    if(full_name.empty())
    {
        return false;
    }
    size_t i = full_name.find_first_of(".");
    if(i == full_name.npos)
    {
        ERRORLOG("not find . in full name [%s]", full_name);
        return false;
    }
    service_name = full_name.substr(0, i);
    method_name = full_name.substr(i+1, full_name.length()- i - 1);
    INFOLOG("parse service_name [%s] and method_name[%s] from full_name[%s]", service_name.c_str(), method_name.c_str(), full_name.c_str());
    return true;
}
void rpcDispatcher::set_tinyPB_error(std::shared_ptr<tinyPBProtocol> msg, int32_t err_code, const std::string err_info)
{
    msg->m_err_code = err_code;
    msg->m_err_info = err_info;
    msg->m_err_info_len = err_info.length();
}
    
}