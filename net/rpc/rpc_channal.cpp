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
#include "../../include/error_code.h"
namespace kabi
{
rpcChannel::rpcChannel(netAddr::s_ptr peer_addr):m_peer_addr(peer_addr)
{

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
    //request 的序列化
    if(!request->SerializeToString(&req_protocol->m_pb_data))
    {
        std::string error_info = "failed to serialize";
        m_controller->SetError(ERROR_FAILED_SERIALIZE, error_info);
        ERRORLOG("[%s] | %s, origin request [%s]", req_protocol->m_msg_id.c_str(), error_info.c_str(), request->ShortDebugString().c_str());
        return;
    }
    tcpClient client(m_peer_addr);
    client.tcp_connect([&client, req_protocol,request, &done](){
        client.write_msg(req_protocol, [&client, req_protocol, request, &done](abstractProtocol::s_ptr)
        {
            INFOLOG("[%s] | send request success, call method name[%s], origin request[%s]", req_protocol->m_msg_id, req_protocol->m_method_name.c_str(),
                request->ShortDebugString().c_str());
            client.read_msg(req_protocol->m_msg_id, [&done](abstractProtocol::s_ptr msg_ptr){
                std::shared_ptr<kabi::tinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<kabi::tinyPBProtocol>(msg_ptr);
                INFOLOG("[%s] | get response , call method name [%s]", rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str());
                if(done)
                {
                    done->Run();
                }
            });
        });
        

    });

}
}