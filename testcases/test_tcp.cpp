#include "include/log.h"
#include "net/tcp/net_addr.h"
#include "net/tcp/tcp_server.h"
#include <memory>
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
    //kabi::ipNetAddr addr("127.0.0.1", 12345);
    //DEBUGLOG("create addr %s", addr.toString().c_str());
    test_tcp_server();
    return 0;
}