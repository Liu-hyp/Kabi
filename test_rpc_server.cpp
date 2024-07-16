#include "include/log.h"
#include "net/tcp/net_addr.h"
#include "net/tcp/tcp_server.h"
#include <memory>
#include <google/protobuf/service.h>
#include "order.pb.h"
#include "net/rpc/rpc_dispatcher.h"
class OrderImpl : public Order{
public:
    void makeOrder(google::protobuf::RpcController* controller,
                       const ::makeOrderRequest* request,
                       ::makeOrderResponse* response,
                       ::google::protobuf::Closure* done)
                       {
                            if(request->price() < 10)
                            {
                                response->set_ret_code(-1);
                                response->set_res_info("short balance");
                                return;
                            }
                            else
                            {
                                response->set_order_id("2024");
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
int main()
{
    std::string xmlfile = "/mnt/hgfs/Share/Kabi/kabi.xml";
    kabi::config::set_global_config(xmlfile.c_str());
    kabi::logger::init_global_logger();
    std::shared_ptr<OrderImpl> order_ptr = std::make_shared<OrderImpl>();
    kabi::rpcDispatcher::get_rpc_dispatcher()->register_service(order_ptr);
    //test_connect();
    test_tcp_server();
    return 0;
}