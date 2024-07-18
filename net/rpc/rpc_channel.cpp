#include "rpc_channel.h"
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "../coder/tinypb_protocol.h"
#include "../../include/msg_util.h"
#include "rpc_channel.h"
#include "rpc_controller.h"
#include "../tcp/tcp_client.h"
#include <memory>
#include <string>
#include "../../include/error_code.h"
namespace kabi
{
rpcChannel::rpcChannel(netAddr::s_ptr peer_addr):m_peer_addr(peer_addr)
{
    m_client = std::make_shared<tcpClient>(m_peer_addr);
}
rpcChannel::~rpcChannel()
{

}
void rpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                            google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                            google::protobuf::Message* response, google::protobuf::Closure* done)
{
    std::shared_ptr<kabi::tinyPBProtocol> req_protocol = std::make_shared<kabi::tinyPBProtocol>();
    rpcController* m_controller = dynamic_cast<rpcController*>(controller);
    if(m_controller == NULL)
    {
        ERRORLOG("failed to callmethod, rpcController convert error");
        return;
    }
    
    if(m_controller->GetMsgId().empty())
    {
        req_protocol->m_msg_id = MsgIDUtil::GenMsgID();
        m_controller->SetMsgId(req_protocol->m_msg_id);
    }
    else
    {
        req_protocol->m_msg_id = m_controller->GetMsgId();
    }
    req_protocol->m_method_name = method->full_name();
    INFOLOG("%s | call method name [%s]", req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str());
    if(!m_is_init)
    {
        std::string error_info = "rpcChannel not init";
        m_controller->SetError(ERROR_RPC_CHANNEL_INIT, error_info);
        ERRORLOG("[%s] | %s, origin request [%s]", req_protocol->m_msg_id.c_str(), error_info.c_str(), request->ShortDebugString().c_str());
        return;
    }
    //request 的序列化
    if(!request->SerializeToString(&req_protocol->m_pb_data))
    {
        std::string error_info = "failed to serialize";
        m_controller->SetError(ERROR_FAILED_SERIALIZE, error_info);
        ERRORLOG("[%s] | %s, origin request [%s]", req_protocol->m_msg_id.c_str(), error_info.c_str(), request->ShortDebugString().c_str());
        return;
    }
    s_ptr channel = shared_from_this();
    
    m_client->tcp_connect([req_protocol, channel]()mutable {
        rpcController* m_controller = dynamic_cast<rpcController*>(channel->getController());
        if(channel->getTcpClient()->get_connect_error_code() != 0)
        {
            m_controller->SetError(channel->getTcpClient()->get_connect_error_code(), channel->getTcpClient()->get_connect_error_info());
            ERRORLOG("[%s] | connect error, error code [%d], error info [%s], peer_addr [%s]", req_protocol->m_msg_id.c_str(),
                m_controller->GetErrorCode(), m_controller->GetErrorInfo().c_str(), channel->getTcpClient()->get_peer_addr()->toString().c_str());
            return;
        }
        channel->getTcpClient()->write_msg(req_protocol, [req_protocol, channel, m_controller](abstractProtocol::s_ptr)mutable
        {
            INFOLOG("[%s] | send request success, call method name[%s], peer_addr[%s], local_addr[%s]", req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str(),
                channel->getTcpClient()->get_peer_addr()->toString().c_str(), channel->getTcpClient()->get_local_addr()->toString().c_str());
            channel->getTcpClient()->read_msg(req_protocol->m_msg_id, [channel, m_controller](abstractProtocol::s_ptr msg_ptr) mutable {
                std::shared_ptr<kabi::tinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<kabi::tinyPBProtocol>(msg_ptr);
                INFOLOG("[%s] | success get response , call method name [%s], peer_addr[%s], local_addr[%s]", rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str(),
                    channel->getTcpClient()->get_peer_addr()->toString().c_str(), channel->getTcpClient()->get_local_addr()->toString().c_str());
                
                if(!channel->getResponse()->ParseFromString(rsp_protocol->m_pb_data))
                {
                    ERRORLOG("[%s] | serialize error", rsp_protocol->m_msg_id.c_str());
                    m_controller->SetError(ERROR_FAILED_SERIALIZE, "serialize error");
                    return;
                }
                if(rsp_protocol->m_err_code != 0)
                {
                    ERRORLOG("[%s] | call rpc failed, error code[%d], error info[%s]", rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_err_code, rsp_protocol->m_err_info.c_str());
                    m_controller->SetError(rsp_protocol->m_err_code, rsp_protocol->m_err_info);
                    return;
                }
                INFOLOG("[%s] | call rpc sucess, call method name[%s], peer_addr[%s], local_addr[%s]", rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str(),
                    channel->getTcpClient()->get_peer_addr()->toString().c_str(), channel->getTcpClient()->get_local_addr()->toString().c_str());
                if(channel->getClosure())
                {
                    channel->getClosure()->Run();
                }
                channel.reset();
            });
        });
        

    });

}
void rpcChannel::rpc_channel_init(controller_s_ptr controller, message_s_ptr req, message_s_ptr rsp, closure_s_ptr done)
{
    if(m_is_init)
    {
        return;
    }
    m_controller = controller;
    m_request = req;
    m_response = rsp;
    m_closure = done;
    m_is_init = true;
}
google::protobuf::RpcController* rpcChannel::getController()
{
    return m_controller.get();
}

google::protobuf::Message* rpcChannel::getRequest()
{
    return m_request.get();
}

google::protobuf::Message* rpcChannel::getResponse()
{
    return m_response.get();
}

google::protobuf::Closure* rpcChannel::getClosure()
{
    return m_closure.get();
}

tcpClient* rpcChannel::getTcpClient()
{
    return m_client.get();
}
}