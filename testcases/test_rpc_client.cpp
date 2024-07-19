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
#include "order.pb.h"
#include "../kabisrc/net/rpc/rpc_channel.h"
#include "../kabisrc/net/rpc/rpc_controller.h"
#include "../kabisrc/net/rpc/rpc_closure.h"
void test_tcp_client()
{
    kabi::ipNetAddr::s_ptr addr = std::make_shared<kabi::ipNetAddr>("192.168.247.128", 8080);
    kabi::tcpClient client(addr);
    client.tcp_connect([addr, &client](){
        DEBUGLOG("connect to [%s] success", addr->toString().c_str());
        //std::shared_ptr<kabi::stringProtocol> message = std::make_shared<kabi::stringProtocol>();
        std::shared_ptr<kabi::tinyPBProtocol> message = std::make_shared<kabi::tinyPBProtocol>();
        //message->info = "hello my kabi!";
        message->m_pb_data = "test pb data";
        message->m_msg_id = "123456";

        makeOrderRequest request;
        request.set_price(100);
        request.set_goods("apple");
        if(!request.SerializeToString(&(message->m_pb_data)))
        {
            //将整个request对象转化为pb结构体
            ERRORLOG("serialize error");
            return;
        }
        message->m_method_name = "Order.makeOrder";
        client.write_msg(message, [request](kabi::abstractProtocol::s_ptr msg_ptr){
            DEBUGLOG("send message success, request[%s]", request.ShortDebugString().c_str());
        });
        client.read_msg("123456", [](kabi::abstractProtocol::s_ptr msg_ptr){
            //std::shared_ptr<kabi::stringProtocol>message = std::dynamic_pointer_cast<kabi::stringProtocol>(msg_ptr); 
            std::shared_ptr<kabi::tinyPBProtocol> message = std::dynamic_pointer_cast<kabi::tinyPBProtocol>(msg_ptr);
            DEBUGLOG("msg_id %s, get response %s", message->m_msg_id.c_str(), message->m_pb_data.c_str());
            makeOrderResponse response;
            if(!response.ParseFromString(message->m_pb_data))
            {
                ERRORLOG("deserialize error");
                return;
            }
            DEBUGLOG("get response success, response[%s]", response.ShortDebugString().c_str());
        });
    });
}
void test_rpc_channel()
{
    //kabi::ipNetAddr::s_ptr addr = std::make_shared<kabi::ipNetAddr>("192.168.247.128", 8080);
    //std::shared_ptr<kabi::rpcChannel> channel = std::make_shared<kabi::rpcChannel>(addr);
    //std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();
    NEWRPCCHANNEL("192.168.247.128:8080", channel);
    NEWMESSAGE(makeOrderRequest, request);
    NEWMESSAGE(makeOrderResponse, response);
    request->set_price(100);
    request->set_goods("apple");
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
    CALLRPRC("192.168.247.128:8080", makeOrder, controller, request, response, closure);
}
int main()
{
    std::string xmlfile = "/mnt/hgfs/Share/Kabi/kabi_client.xml";
    kabi::config::set_global_config(NULL);
    
    kabi::logger::init_global_logger(0);
    //test_connect();
    //test_tcp_client();
    test_rpc_channel();
    return 0;
}