#ifndef KABI_NET_EVENTLOOP_H
#define KABI_NET_EVENTLOOP_H
#include <pthread.h>
#include <set>
#include <sys/socket.h>
#include "../include/log.h"
#include "../include/util.h"
#include <functional>
#include <sys/epoll.h>
#include <queue>
#include <sys/eventfd.h>
#include "../include/mutex.h"
#include "fd_event.h"
#include <string.h>
#include "wakeup_fd_event.h"
#include "timer.h"
namespace kabi{
class eventloop
{
public:
    eventloop();
    ~eventloop();
    void loop();
    void wakeup();
    void stop();
    void add_epoll_event(fdEvent* event);
    void del_epoll_event(fdEvent* event);
    bool is_inloop_thread();
    void add_task(std::function<void()> cb, bool is_wake_up = false);
    void add_timer_event(timerEvent::s_ptr event);
private:
    void deal_wakeup();
    void init_wakeup_fd_event();
    void init_timer();
private:
    pid_t m_pid {0};
    pid_t m_thread_id {0};
    int m_epoll_fd {0};
    int m_wakeup_fd {0};
    wakeUpFdEvent* m_wakeup_fd_event {nullptr};
    bool m_stop_flag {false};
    kabi::mutex m_mutex;
    std::set<int>m_listen_fds; //当前监听的所有套接字
    std::queue<std::function<void()>>m_pending_tasks;
    timer* m_timer {nullptr};
};
}
#endif //KABI_NET_EVENTLOOP_H