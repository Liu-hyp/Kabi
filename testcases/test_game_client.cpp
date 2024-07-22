#include <pthread.h>
#include "../kabisrc/include/log.h"
#include "../kabisrc/include/config.h"
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory>
#include "../kabisrc/net/coder/abstract_protocol.h"
#include "../kabisrc/net/tcp/tcp_connection.h"
#include "../kabisrc/net/tcp/net_addr.h"
#include "../kabisrc/net/tcp/tcp_client.h"
//#include "net/coder/string_coder.h"
#include "../kabisrc/net/coder/abstract_protocol.h"
#include "../kabisrc/net/coder/tinypb_coder.h"
#include "../kabisrc/net/coder/tinypb_protocol.h"
#include "../kabisrc/net/game/GameMsg.pb.h"
#include "../kabisrc/net/rpc/rpc_channel.h"
#include "../kabisrc/net/rpc/rpc_controller.h"
#include "../kabisrc/net/rpc/rpc_closure.h"
void test_tcp_client()
{
    NEWRPCCHANNEL("192.168.247.128:8080", channel);
    NEWMESSAGE(SyncPid, request);
    NEWMESSAGE(SyncPidResponse, response);
    request->set_pid(channel->getGameRole()->m_fd);
    request->set_username(channel->getGameRole()->m_username);
    //std::shared_ptr<makeOrderResponse> response = std::make_shared<makeOrderResponse>();
    //std::shared_ptr<kabi::rpcController> controller = std::make_shared<kabi::rpcController>();
    NEWRPCCONTROLLER(controller);
    controller->SetMsgId("123456777");
    controller->SetTimeout(10000);
    std::shared_ptr<kabi::RpcClosure> closure = std::make_shared<kabi::RpcClosure>([request, response, channel, controller]() mutable {
        if(controller->GetErrorCode() == 0)
        {
            INFOLOG("call rpc success, request[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
            //执行业务逻辑
            //if(response->order_id() == xxx)
            if(response->state() == Status::OK)
            {
                channel->getGameRole()->init();
                printf("成功添加player[%s]进入小白的世界， id[%d]", channel->getGameRole()->m_username.c_str(), channel->getGameRole()->m_fd);
            }else
            {
                printf("失败了");
            }
        }
        else
        {
            ERRORLOG("call rpc failed, request[%s], error code[%d], error info[%s]", request->ShortDebugString().c_str(), controller->GetErrorCode(),
                controller->GetErrorInfo().c_str());
        }
        
        channel->getTcpClient()->stop();
        channel.reset();
    });
    // channel->rpc_channel_init(controller, request, response, closure);
    // Order_Stub stub(channel.get());
    // //在这里调用rpc时，虽然是异步的。但也不希望无限期的去等他的结果，所以说给一个超时时间
    // stub.makeOrder(controller.get(), request.get(), response.get(), closure.get());
    //CALLRPRC("192.168.247.128:8080", pb::Login_Stub, makeLogin, controller, request, response, closure);
    channel->rpc_channel_init(controller, request, response, closure);
    Login_Stub stub(channel.get());
    //在这里调用rpc时，虽然是异步的。但也不希望无限期的去等他的结果，所以说给一个超时时间
    stub.makeLogin(controller.get(), request.get(), response.get(), closure.get());
}
int main()
{
    //std::string xmlfile = "/mnt/hgfs/Share/Kabi/conf/kabi_client.xml";
    kabi::config::set_global_config(NULL);
    kabi::logger::init_global_logger(0);
    test_tcp_client();
    return 0;
}