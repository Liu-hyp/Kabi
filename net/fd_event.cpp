#include "fd_event.h"
#include "../include/log.h"
namespace kabi
{
fdEvent::fdEvent(int fd):m_fd(fd){};
fdEvent::~fdEvent(){};
std::function<void()> fdEvent::handler(FdTriggerEvent event_type)
{
    if(event_type == FdTriggerEvent::IN_EVENT)
    {
        return m_read_callback;
    }
    else if(event_type == FdTriggerEvent::OUT_EVENT)
    {
        return m_write_callback;
    }

}
void fdEvent::listen(FdTriggerEvent event_type, std::function<void()> callback)
{
    if(event_type == FdTriggerEvent::IN_EVENT)
    {
        m_listen_events.events |= EPOLLIN;
        m_read_callback = callback;
    }
    else if(event_type == FdTriggerEvent::OUT_EVENT)
    {
        m_listen_events.events |= EPOLLOUT;        
        m_write_callback = callback;       
    }
    m_listen_events.data.ptr = this;
    
}

}