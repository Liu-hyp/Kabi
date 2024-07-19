#ifndef KABI_NET_RPC_RPC_CHANNEL_H
#define KABI_NET_RPC_RPC_CHANNEL_H

#include <google/protobuf/service.h>
#include <memory>
#include "../tcp/net_addr.h"
#include "../tcp/tcp_client.h"
#include "../timer_event.h"

namespace kabi {

#define NEWMESSAGE(type, var_name) \
  std::shared_ptr<type> var_name = std::make_shared<type>(); \

#define NEWRPCCONTROLLER(var_name) \
  std::shared_ptr<kabi::rpcController> var_name = std::make_shared<kabi::rpcController>(); \

#define NEWRPCCHANNEL(addr, var_name) \
  std::shared_ptr<kabi::rpcChannel> var_name = std::make_shared<kabi::rpcChannel>(std::make_shared<kabi::ipNetAddr>(addr)); \

#define CALLRPRC(addr, stub_name, method_name, controller, request, response, closure) \
  { \
  NEWRPCCHANNEL(addr, channel); \
  channel->rpc_channel_init(controller, request, response, closure); \
  stub_name(channel.get()).method_name(controller.get(), request.get(), response.get(), closure.get()); \
  } \

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

    timerEvent::s_ptr get_timer_event();
private:
    netAddr::s_ptr m_peer_addr {nullptr};
    netAddr::s_ptr m_local_peer {nullptr};
    controller_s_ptr m_controller {nullptr};
    message_s_ptr m_request {nullptr};
    message_s_ptr m_response {nullptr};
    closure_s_ptr m_closure {nullptr};
    tcpClient::s_ptr m_client {nullptr};
    bool m_is_init {false};
    timerEvent::s_ptr m_timer_event{nullptr};

};

}


#endif