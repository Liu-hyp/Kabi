#ifndef KABI_NET_RPC_RPC_CHANNEL_H
#define KABI_NET_RPC_RPC_CHANNEL_H

#include <google/protobuf/service.h>
#include <memory>
#include "../tcp/net_addr.h"
#include "../tcp/tcp_client.h"
#include "../timer_event.h"

namespace kabi {
class rpcChannel : public google::protobuf::RpcChannel, public std::enable_shared_from_this<rpcChannel> 
{
public:
    typedef std::shared_ptr<rpcChannel> s_ptr;
    typedef std::shared_ptr<google::protobuf::RpcController> controller_s_ptr;
    typedef std::shared_ptr<google::protobuf::Message> message_s_ptr;
    typedef std::shared_ptr<google::protobuf::Closure> closure_s_ptr;
public:

    rpcChannel(netAddr::s_ptr peer_addr);
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                            google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                            google::protobuf::Message* response, google::protobuf::Closure* done);
    void rpc_channel_init(controller_s_ptr controller, message_s_ptr req, message_s_ptr rsp, closure_s_ptr done);
    ~rpcChannel();
    google::protobuf::RpcController* getController(); 

    google::protobuf::Message* getRequest();

    google::protobuf::Message* getResponse();

    google::protobuf::Closure* getClosure();

    tcpClient* getTcpClient();
private:
    netAddr::s_ptr m_peer_addr {nullptr};
    netAddr::s_ptr m_local_peer {nullptr};
    controller_s_ptr m_controller {nullptr};
    message_s_ptr m_request {nullptr};
    message_s_ptr m_response {nullptr};
    closure_s_ptr m_closure {nullptr};
    tcpClient::s_ptr m_client {nullptr};
    bool m_is_init {false};

};

}


#endif