#ifndef KABI_NET_FD_EVENT_H
#define KABI_NET_FD_EVENT_H
#include <functional>
#include <sys/epoll.h>
namespace kabi
{
class fdEvent
{
public:
    enum class FdTriggerEvent
    {
        IN_EVENT = EPOLLIN,
        OUT_EVENT = EPOLLOUT,
    };
    fdEvent(int fd);
    fdEvent();
    ~fdEvent();
    void set_nonblock();
    std::function<void()> handler(FdTriggerEvent event_type);

    void listen(FdTriggerEvent event_type, std::function<void()> callback);
    //取消监听
    void cancel(FdTriggerEvent event_type);
    int get_fd() const{return m_fd;}
    epoll_event get_epoll_event()
    {
        return m_listen_events;
    }
protected:
    int m_fd {-1};
    epoll_event m_listen_events;
    std::function<void()> m_read_callback;
    std::function<void()> m_write_callback;
};
}
#endif //KABI_NET_FD_EVENT_H