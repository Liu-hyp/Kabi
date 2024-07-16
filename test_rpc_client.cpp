#include <pthread.h>
#include "include/log.h"
#include "include/config.h"
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory>
#include "net/coder/abstract_protocol.h"
#include "net/tcp/tcp_connection.h"
#include "net/tcp/net_addr.h"
#include "net/tcp/tcp_client.h"
//#include "net/coder/string_coder.h"
#include "net/coder/abstract_protocol.h"
#include "net/coder/tinypb_coder.h"
#include "net/coder/tinypb_protocol.h"
#include "order.pb.h"
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
        message->m_req_id = "123456";

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
            DEBUGLOG("req_id %s, get response %s", message->m_req_id.c_str(), message->m_pb_data.c_str());
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
int main()
{
    std::string xmlfile = "/mnt/hgfs/Share/Kabi/kabi.xml";
    kabi::config::set_global_config(xmlfile.c_str());
    kabi::logger::init_global_logger();
    //test_connect();
    test_tcp_client();
    return 0;
}