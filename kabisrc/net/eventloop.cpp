#include "eventloop.h"
#define ADD_TO_EPOLL() \
    auto it = m_listen_fds.find(event->get_fd()); \
    int op = EPOLL_CTL_ADD; \
    if(it != m_listen_fds.end()) \
    { \
        op = EPOLL_CTL_MOD; \
    } \
    epoll_event tmp = event->get_epoll_event(); \
    INFOLOG("epoll_event.events = %d", (int)tmp.events); \
    int rt = epoll_ctl(m_epoll_fd, op, event->get_fd(), &tmp); \
    if(rt == -1) \
    { \
        ERRORLOG("failed epoll_ctl when add fd %d, errno = %d, error = %s", event->get_fd(), errno, strerror(errno)); \
    } \
    m_listen_fds.insert(event->get_fd()); \
    DEBUGLOG("add event success, fd[%d]", event->get_fd()); \

#define DEL_TO_EPOLL() \
    auto it = m_listen_fds.find(event->get_fd()); \
    if(it == m_listen_fds.end()) \
    { \
        return; \
    } \
    int op = EPOLL_CTL_DEL; \
    epoll_event tmp = event->get_epoll_event(); \
    int rt = epoll_ctl(m_epoll_fd, op, event->get_fd(), &tmp); \
    if(rt == -1) \
    { \
        ERRORLOG("failed to epoll ctl when del, errno = %d, error = %s", errno, strerror(errno)); \
    } \
    m_listen_fds.erase(event->get_fd()); \
    DEBUGLOG("del event success, fd[%d]", event->get_fd()); \

namespace kabi{
static thread_local eventloop* t_current_eventloop = NULL;
static int g_global_max_timeout = 10000;
static int g_epoll_max_event = 10; //单次最大的监听事件
eventloop::eventloop()
{
    if(t_current_eventloop != NULL)
    {
        printf("failed to create event loop, this thread has created event loop.");
        exit(0);
    }
    m_thread_id = get_thread_id();
    m_epoll_fd = epoll_create(10);
    if(m_epoll_fd == -1)
    {
        ERRORLOG("failed to  create event loop, epoll_create error, error info[%d]", errno);
        exit(0);
    }
    init_wakeup_fd_event();
    init_timer();
    INFOLOG("succ create event loop in thread %d", m_pid);
    t_current_eventloop = this;
}
eventloop::~eventloop()
{
    close(m_epoll_fd);
    if(m_wakeup_fd_event)
    {
        delete m_wakeup_fd_event;
        m_wakeup_fd_event = nullptr;
    }
    if(m_timer)
    {
        delete m_timer;
        m_timer = nullptr;
    }

}
void eventloop::loop()
{
    m_is_looping = true;
    while(!m_stop_flag)
    {
        scope_mutext<mutex> locker(m_mutex);
        std::queue<std::function<void()>>tmp_tasks;
        m_pending_tasks.swap(tmp_tasks);
        locker.unlock();
        while(!tmp_tasks.empty())
        {
            std::function<void()> cb = tmp_tasks.front();
            tmp_tasks.pop();
            if(cb)
            {
                cb();
            }
            
        }
        //如果有定时任务需要执行，那么执行
        //1.怎么判断一个定时任务需要执行？(now() > TimerEvent.arrtive_time)
        //2.arrtive_time 如何让event loop监听
        int timeout = g_global_max_timeout;
        epoll_event result_event[g_epoll_max_event];
        DEBUGLOG("now begin to epoll wait");
        int rt = epoll_wait(m_epoll_fd, result_event, g_epoll_max_event, timeout);
        DEBUGLOG("now end to epoll wait, rt = %d", rt);
        if(rt < 0)
        {
            ERRORLOG("epoll_wait error, error = ", errno);
        }
        else{
            for(int i = 0; i < rt; ++i)
            {
                epoll_event trigger_event = result_event[i];
                fdEvent* fd_event = static_cast<fdEvent*>(trigger_event.data.ptr);
                if(fd_event == NULL)
                {
                    continue;
                }
                if(trigger_event.events & EPOLLIN)
                {
                    DEBUGLOG("fd[%d] trigger in EPOLLIN event", fd_event->get_fd());
                    add_task(fd_event->handler(fdEvent::FdTriggerEvent::IN_EVENT));                  
                }
                if(trigger_event.events & EPOLLOUT)
                {
                    DEBUGLOG("fd[%d] trigger in EPOLLOUT event", fd_event->get_fd());
                    add_task(fd_event->handler(fdEvent::FdTriggerEvent::OUT_EVENT));
                }
                if(trigger_event.events & EPOLLERR)
                {
                    DEBUGLOG("fd[%d] trigger in EPOLLERR event", fd_event->get_fd());
                    //删除出错的套接字
                    del_epoll_event(fd_event);
                    if(fd_event->handler(fdEvent::FdTriggerEvent::ERROR_EVENT) != nullptr)
                    {
                        DEBUGLOG("fd %d add error callback", fd_event->get_fd());
                        add_task(fd_event->handler(fdEvent::FdTriggerEvent::OUT_EVENT));
                    }
                }
            }
        }        
    }
}
bool eventloop::is_looping()
{
    return m_is_looping;
}
void eventloop::wakeup()
{
    m_wakeup_fd_event->wakeup();

}
void eventloop::deal_wakeup()
{

}
void eventloop::stop()
{
    m_stop_flag = true;
    wakeup();
}
void eventloop::add_epoll_event(fdEvent* event)
{
    if(is_inloop_thread()) //为了保证线程安全
    {
        ADD_TO_EPOLL();    
    }else{
        auto cb = [this, event]() {
            ADD_TO_EPOLL();
        };
        add_task(cb, true);
    }
}
void eventloop::del_epoll_event(fdEvent* event)
{
    if(is_inloop_thread())
    {
        DEL_TO_EPOLL();
    }
    else{
        auto cb = [this, event](){
            DEL_TO_EPOLL();           
        };
        add_task(cb, true);
    }

}
bool eventloop::is_inloop_thread()
{
    //防止条件竞争，判断是当前线程在添加epoll事件
    return get_thread_id() == m_thread_id;
}
void eventloop::add_task(std::function<void()> cb, bool is_wake_up)
{
    scope_mutext<mutex>locker(m_mutex);
    m_pending_tasks.push(cb);
    locker.unlock();
    if(is_wake_up)
    {
        wakeup();
    }
}
void eventloop::init_wakeup_fd_event()
{
    m_wakeup_fd = eventfd(0, EFD_NONBLOCK); //设置非阻塞
    if(m_wakeup_fd < 0)
    {
        ERRORLOG("failed to create event loop, eventfd create error, error info[%d]", errno);
        exit(0);
    }
    m_wakeup_fd_event = new wakeUpFdEvent(m_wakeup_fd);
    DEBUGLOG("success create wake up fd event");
    auto m_read_callback = [this](){
        char buf[8];
        while(read(m_wakeup_fd, buf, 8) != -1 && errno != EAGAIN) //非阻塞 循环读 
        {
        }
        DEBUGLOG("read full bytes from wake up fd[%d]", m_wakeup_fd);
    };
    m_wakeup_fd_event->listen(fdEvent::FdTriggerEvent::IN_EVENT, m_read_callback); //监听可读事件
    add_epoll_event(m_wakeup_fd_event);
}
void eventloop::init_timer()
{
    m_timer = new timer();
    add_epoll_event(m_timer);
}
void eventloop::add_timer_event(timerEvent::s_ptr event)
{
    m_timer->add_timer_event(event);
}
eventloop* eventloop::get_current_event_loop()
{
    if(t_current_eventloop)
    {
        return t_current_eventloop;
    }t_current_eventloop = new eventloop();
    return t_current_eventloop;
}
}