#include "../kabisrc/include/log.h"
#include "../kabisrc/net/tcp/net_addr.h"
#include "../kabisrc/net/tcp/tcp_server.h"
#include <memory>
#include <google/protobuf/service.h>
#include "../kabisrc/net/rpc/rpc_dispatcher.h"
#include "../kabisrc/net/game/GameMsg.pb.h"

class LoginImpl : public Login{
public:
void makeLogin(google::protobuf::RpcController* controller,
                       const SyncPid* request,
                       SyncPidResponse* response,
                       ::google::protobuf::Closure* done)
                       {
                            //APPLOG只能在rpc方法里面用                           
                            response->set_state(Status::OK);
                            APPDEBUGLOG("call makeOrder success");
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
    // if(argc != 2)
    // {
    //     printf("start test_rpc_server error, argc not 2 \n");
    //     printf("Start like this: \n");
    //     printf("./test_rpc_server ../conf/rocket.xml \n");
    //     return 0;
    // }
    std::string xmlfile = "/mnt/hgfs/Share/Kabi/conf/kabi.xml";
    kabi::config::set_global_config(xmlfile.c_str());
    kabi::logger::init_global_logger();
    std::shared_ptr<LoginImpl> login_ptr = std::make_shared<LoginImpl>();
    kabi::rpcDispatcher::get_rpc_dispatcher()->register_service(login_ptr);
    kabi::ipNetAddr::s_ptr addr = std::make_shared<kabi::ipNetAddr>("192.168.247.128", kabi::config::get_global_config()->m_port);
    kabi::tcpServer tcp_server(addr);
    tcp_server.start();
    return 0;
}