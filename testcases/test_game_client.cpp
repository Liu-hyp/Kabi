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
#include "../kabisrc/net/game/game_msg.h"
#include "../kabisrc/net/game/game_role.h"
#include "../kabisrc/net/game/AOI.h"
void test_tcp_client()
{
    NEWRPCCHANNEL("192.168.247.128:8080", channel);
    // NEWMESSAGE(SyncPid, request);
    // NEWMESSAGE(SyncPidResponse, response);
    // channel->getGameRole()->create_id_name_login(request);
    // NEWRPCCONTROLLER(controller);
    // controller->SetMsgId("123456777");
    // controller->SetTimeout(10000);
    // std::shared_ptr<kabi::RpcClosure> closure = std::make_shared<kabi::RpcClosure>([request, response, channel, controller]() mutable {
    //     if(controller->GetErrorCode() == 0)
    //     {
    //         INFOLOG("call rpc success, request[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
    //         //执行业务逻辑
    //         //if(response->order_id() == xxx)
    //         if(response->state() == Status::OK)
    //         {
    //             channel->getGameRole()->init();
    //             printf("成功添加player[%s]进入小白的世界， id[%d]", channel->getGameRole()->m_username.c_str(), channel->getGameRole()->m_id);
    //         }else
    //         {
    //             printf("失败了");
    //         }
    //     }
    //     else
    //     {
    //         ERRORLOG("call rpc failed, request[%s], error code[%d], error info[%s]", request->ShortDebugString().c_str(), controller->GetErrorCode(),
    //             controller->GetErrorInfo().c_str());
    //     }
    //     //channel->getTcpClient()->stop();
    //     //channel.reset();
    // });
    // // channel->rpc_channel_init(controller, request, response, closure);
    // // Order_Stub stub(channel.get());
    // // //在这里调用rpc时，虽然是异步的。但也不希望无限期的去等他的结果，所以说给一个超时时间
    // // stub.makeOrder(controller.get(), request.get(), response.get(), closure.get());
    // //CALLRPRC("192.168.247.128:8080", pb::Login_Stub, makeLogin, controller, request, response, closure);
    // channel->rpc_channel_init(controller, request, response, closure);
    // Login_Stub stub(channel.get());
    // //在这里调用rpc时，虽然是异步的。但也不希望无限期的去等他的结果，所以说给一个超时时间
    // stub.makeLogin(controller.get(), request.get(), response.get(), closure.get());  
    channel->getGameRole()->init();
    NEWMESSAGE(SyncPlayers, new_request);
    NEWMESSAGE(SyncPlayersResponse, new_response);
    channel->getGameRole()->create_srd_players(new_request);
    NEWRPCCONTROLLER(new_controller);
    new_controller->SetMsgId("123456777888");
    new_controller->SetTimeout(10000);
    std::shared_ptr<kabi::RpcClosure> new_closure = std::make_shared<kabi::RpcClosure>([new_request, new_response, channel, new_controller]() mutable {
        if(new_controller->GetErrorCode() == 0)
        {
            INFOLOG("call rpc success, request[%s], response[%s]", new_request->ShortDebugString().c_str(), new_response->ShortDebugString().c_str());
            //执行业务逻辑
            //if(response->order_id() == xxx)
            if(new_response->state() == Status::OK)
            {
                printf("player[%s]", channel->getGameRole()->m_username.c_str());
            }else
            {
                printf("失败了\n");
            }
        }
        else
        {
            ERRORLOG("call rpc failed, request[%s], error code[%d], error info[%s]", new_request->ShortDebugString().c_str(), new_controller->GetErrorCode(),
                new_controller->GetErrorInfo().c_str());
        }
        channel->getTcpClient()->stop();
        channel.reset();
    });
    channel->rpc_channel_init(new_controller, new_request, new_response, new_closure);
    SyncSrdPlayer_Stub stub1(channel.get());
    //在这里调用rpc时，虽然是异步的。但也不希望无限期的去等他的结果，所以说给一个超时时间
    stub1.makeSrdPlayer(new_controller.get(), new_request.get(), new_response.get(), new_closure.get());
}
int main()
{
    //std::string xmlfile = "/mnt/hgfs/Share/Kabi/conf/kabi_client.xml";
    kabi::config::set_global_config(NULL);
    kabi::logger::init_global_logger(0);
    kabi::AOIWorld::init_global_world(20, 200, 50, 230, 6, 6);
    std::shared_ptr<kabi::GameRole> p1 = std::make_shared<kabi::GameRole>(100, "芒果", 91, 118, 0, 0);
    std::shared_ptr<kabi::GameRole> p2 = std::make_shared<kabi::GameRole>(101, "小白", 147, 133, 0, 0);
    kabi::AOIWorld::get_global_world()->add_player(p1);
    kabi::AOIWorld::get_global_world()->add_player(p2);
    test_tcp_client();
    //test_game_syncPlayer();
    return 0;
}