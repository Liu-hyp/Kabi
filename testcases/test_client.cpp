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

void test_connect()
{
    //调用connect连接server
    //send一个字符串
    //等待read返回结果
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        ERRORLOG("invalid fd %d", fd);
        exit(0);
    }
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_aton("192.168.247.128", &server_addr.sin_addr);
    int rt = connect(fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    std::string msg = "hello kabi!";
    rt = write(fd, msg.c_str(), msg.length());
    DEBUGLOG("success write %d bytes, [%s]", rt, msg.c_str());
    char buf[100];
    memset(buf, 0, sizeof(buf));
    rt = read(fd, buf, 100);
    DEBUGLOG("success read %d bytes, [%s]", rt, std::string(buf).c_str());

}
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
        client.write_msg(message, [](kabi::abstractProtocol::s_ptr msg_ptr){
            DEBUGLOG("send message success");
        });
        client.read_msg("123456", [](kabi::abstractProtocol::s_ptr msg_ptr){
            //std::shared_ptr<kabi::stringProtocol>message = std::dynamic_pointer_cast<kabi::stringProtocol>(msg_ptr); 
            std::shared_ptr<kabi::tinyPBProtocol> message = std::dynamic_pointer_cast<kabi::tinyPBProtocol>(msg_ptr);
            DEBUGLOG("msg_id %s, get response %s", message->m_msg_id.c_str(), message->m_pb_data.c_str());
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