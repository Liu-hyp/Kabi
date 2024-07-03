#include "timer_event.h"
#include "../include/log.h"
#include "../include/util.h"

namespace kabi{
timerEvent::timerEvent(int interval, bool is_repeated, std::function<void()> cb)
        : m_interval(interval), m_is_repeated(is_repeated), m_task(cb)
        {
            reset_arrive_time();
        }
timerEvent::~timerEvent()
{
    
}
void timerEvent::reset_arrive_time()
{
    m_arrive_time = get_now_ms() + m_interval;
    DEBUGLOG("success create timer event, will excute at [%lld]", m_arrive_time);
}
}
