#include "../kabisrc/include/log.h"
#include "../kabisrc/net/tcp/net_addr.h"
#include "../kabisrc/net/tcp/tcp_server.h"
#include <memory>
#include <google/protobuf/service.h>
#include "order.pb.h"
#include "../kabisrc/net/rpc/rpc_dispatcher.h"
class OrderImpl : public Order{
public:
    void makeOrder(google::protobuf::RpcController* controller,
                       const ::makeOrderRequest* request,
                       ::makeOrderResponse* response,
                       ::google::protobuf::Closure* done)
                       {
                            //APPLOG只能在rpc方法里面用
                            APPDEBUGLOG("start sleep 5s");
                            sleep(5);
                            APPDEBUGLOG("end sleep 5s");
                            if(request->price() < 10)
                            {
                                response->set_ret_code(-1);
                                response->set_res_info("short balance");
                                return;
                            }
                            else
                            {
                                response->set_order_id("2024");
                                APPDEBUGLOG("call makeOrder success");
                            }

                       }
    
};
void test_tcp_server()
{
    kabi::ipNetAddr::s_ptr addr = std::make_shared<kabi::ipNetAddr>("192.168.247.128", 8080);
    DEBUGLOG("create addr %s", addr->toString().c_str());
    kabi::tcpServer tcp_server(addr);
    tcp_server.start();
}
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("start test_rpc_server error, argc not 2 \n");
        printf("Start like this: \n");
        printf("./test_rpc_server ../conf/rocket.xml \n");
        return 0;
    }
    //std::string xmlfile = "/mnt/hgfs/Share/Kabi/kabi.xml";
    kabi::config::set_global_config(argv[1]);
    kabi::logger::init_global_logger();
    std::shared_ptr<OrderImpl> order_ptr = std::make_shared<OrderImpl>();
    kabi::rpcDispatcher::get_rpc_dispatcher()->register_service(order_ptr);
    kabi::ipNetAddr::s_ptr addr = std::make_shared<kabi::ipNetAddr>("192.168.247.128", kabi::config::get_global_config()->m_port);
    kabi::tcpServer tcp_server(addr);
    tcp_server.start();
    return 0;
}