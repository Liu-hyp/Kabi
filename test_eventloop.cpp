#include "include/config.h"
#include "include/log.h"
#include <thread>
#include "net/fd_event.h"
#include "net/eventloop.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "net/timer_event.h"
#include <memory>

int main()
{
    std::string xmlfile = "/mnt/hgfs/Share/Kabi/kabi.xml";
    kabi::config::set_global_config(xmlfile.c_str());
    kabi::logger::init_global_logger();
    kabi::eventloop* eventloop = new kabi::eventloop();
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
    {
        ERRORLOG("failed to create listenfd");
        exit(0);
    }
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int rt = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
    if(rt == -1)
    {
        ERRORLOG("failed to bind");
        exit(1);
    }
    rt = listen(listenfd, 100);
    if(rt != 0)
    {
        ERRORLOG("failed to listen");
        exit(1);
    }
    kabi::fdEvent event(listenfd);
    event.listen(kabi::fdEvent::FdTriggerEvent::IN_EVENT, [listenfd](){
        sockaddr_in peer_addr;
        memset(&peer_addr, 0, sizeof(peer_addr));
        socklen_t addr_len = sizeof(peer_addr);
        int clientfd = accept(listenfd, reinterpret_cast<sockaddr*>(&peer_addr), &addr_len);
        const char* ip = inet_ntoa(peer_addr.sin_addr);
        int port = ntohs(peer_addr.sin_port);
        DEBUGLOG("success get client fd[%d], peer addr: [%s : %d]", clientfd, ip, port);
    });
    eventloop->add_epoll_event(&event);
    int i = 0;
    kabi::timerEvent::s_ptr timer_event = std::make_shared<kabi::timerEvent>(1000, true, [&i](){
        INFOLOG("trigger timer event, count %d", i++);
    });
    eventloop->add_timer_event(timer_event);
    eventloop->loop();
    return 0;
}