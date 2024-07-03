#include "sys/timerfd.h"
#include "timer.h"
#include "../include/log.h"
#include "../include/util.h"
#include <string.h>
namespace kabi{
timer::timer() : fdEvent()
{
    m_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    DEBUGLOG("success create timer, fd = %d", m_fd);
    //把fd的可读事件放到了eventloop上监听
    listen(fdEvent::FdTriggerEvent::IN_EVENT, std::bind(&timer::on_timer, this));
}
timer::~timer()
{
    
}
void timer::reset_arrive_time()
{
    scope_mutext<mutex> locker(m_mutex);
    auto tmp = m_pending_events;
    locker.unlock();
    if(tmp.size() == 0)return;
    int64_t now = get_now_ms();
    auto it = tmp.begin();
    int64_t interval = 0;
    if(it->second->get_arrive_time() > now)
    {
        interval = it->second->get_arrive_time() - now;
    }
    else{
        interval = 100; //100ms触发定时事件
    }
    timespec ts;
    memset(&ts, 0, sizeof(ts));
    ts.tv_sec = interval/1000;
    ts.tv_nsec = (interval%1000)*1000000;
    itimerspec value;
    memset(&value, 0, sizeof(value));
    value.it_value = ts;
    int rt = timerfd_settime(m_fd, 0, &value, NULL);
    if(rt != 0)
    {
        ERRORLOG("timefd settime error, errno = %d, error = %s", errno, strerror(errno));
    }
    DEBUGLOG("timer reset td %lld", now + interval);
}
void timer::add_timer_event(timerEvent::s_ptr event)
{
    bool is_reset_timerfd = false; //是否需要重新设置超时时间
    scope_mutext<mutex> locker(m_mutex);
    if(m_pending_events.empty())
    {
        is_reset_timerfd = true;
    }
    else
    {
        auto it = m_pending_events.begin();
        if((*it).second->get_arrive_time() > event->get_arrive_time())
        {
            //当前插入的任务的定时事件比队列里所有的时间都要早，需要修改
            is_reset_timerfd = true;
        }
    }
    m_pending_events.emplace(std::make_pair(event->get_arrive_time(), event));
    locker.unlock();
    if(is_reset_timerfd)
    {
        reset_arrive_time();
    }

}

void timer::del_timer_event(timerEvent::s_ptr event)
{   
    event->set_cancel(true);
    scope_mutext<mutex>locker(m_mutex);
    //map key有多个
    auto begin = m_pending_events.lower_bound(event->get_arrive_time());
    auto end = m_pending_events.upper_bound(event->get_arrive_time());
    auto it = begin;
    for(it = begin; it != end; ++it)
    {
        if(it->second == event)
        {
            break;
        }
    }
    if(it != end)
    {
        m_pending_events.erase(it);
    }
    locker.unlock();
    DEBUGLOG("success delete TimeEvent at arrive time %lld", event->get_arrive_time());
}
void timer::on_timer()
{
    //处理缓冲区数据，防止下一次继续触发可读事件
    char buf[8];
    while(1)
    {
        if((read(m_fd, buf, 8) == -1) && errno == EAGAIN)
        {
            break;
        }
    }
    //执行定时任务
    int64_t now = get_now_ms();
    std::vector<timerEvent::s_ptr> tmps;
    std::vector<std::pair<int64_t, std::function<void()>>> tasks;
    scope_mutext<mutex>locker(m_mutex);
    auto it = m_pending_events.begin();
    for(it = m_pending_events.begin(); it != m_pending_events.end(); ++it)
    {
        if(it->first <= now)
        {
            if(!(it->second->is_cancled()))
            {
                tmps.push_back(it->second);
                tasks.push_back(std::make_pair(it->second->get_arrive_time(), it->second->get_call_back()));
            }            
        }
        else{
            break;
        }
    }
    m_pending_events.erase(m_pending_events.begin(), it);
    locker.unlock();
    //需要把重复的Eent。再次添加进m_pending_tasks
    for(auto i = tmps.begin(); i != tmps.end(); ++i)
    {
        if((*i)->is_repeated())
        {
            //调整arrivtime
            (*i)->reset_arrive_time();
            add_timer_event(*i);
        }
    }
    reset_arrive_time();
    for(auto i : tasks)
    {
        if(i.second)
        {
            i.second();
        }
    }
}
}