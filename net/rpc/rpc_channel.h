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
    rpcChannel(netAddr::s_ptr peer_addr);
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                            google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                            google::protobuf::Message* response, google::protobuf::Closure* done);
    ~rpcChannel();
private:
    netAddr::s_ptr m_peer_addr {nullptr};
    netAddr::s_ptr m_local_peer {nullptr};
    
};

}


#endif