#ifndef KABI_NET_TIMER_H
#define KABI_NET_TIMER_H
#include <map>
#include "../include/mutex.h"
#include "fd_event.h"
#include "timer_event.h"
namespace kabi
{
class timer : public fdEvent
{
public:
    timer();
    ~timer();
    void add_timer_event(timerEvent::s_ptr event);
    void del_timer_event(timerEvent::s_ptr event);
    void on_timer(); //当发生了IO事件后eventloop会执行这个函数
private:
    void reset_arrive_time();
private:
    std::multimap<int64_t, timerEvent::s_ptr> m_pending_events;
    mutex m_mutex;
};
}
#endif //KABI_NET_TIMER_H