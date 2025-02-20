#include "fd_event.h"
#include "../include/log.h"
#include <fcntl.h>
namespace kabi
{
fdEvent::fdEvent(int fd):m_fd(fd){
    memset(&m_listen_events, 0 ,sizeof(m_listen_events));
}
fdEvent::fdEvent()
{
    memset(&m_listen_events, 0, sizeof(m_listen_events));
}
fdEvent::~fdEvent(){}
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
    else if(event_type == FdTriggerEvent::ERROR_EVENT)
    {
        return m_error_callback;
    }
    return nullptr;

}
void fdEvent::listen(FdTriggerEvent event_type, std::function<void()> callback, std::function<void()> error_callback /*=nullptr*/)
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
    if(m_error_callback == nullptr)
    {
        m_error_callback = error_callback;
    }else
    {
        m_error_callback = nullptr;
    }
    m_listen_events.data.ptr = this;
    
}
void fdEvent::set_nonblock()
{
    int flag = fcntl(m_fd, F_GETFL, 0);
    if(flag & O_NONBLOCK)
    {
        return;
    }
    fcntl(m_fd, F_SETFL, flag |= O_NONBLOCK);
}
void fdEvent::cancel(FdTriggerEvent event_type)
{
    if(event_type == FdTriggerEvent::IN_EVENT)
    {
        m_listen_events.events &= (~EPOLLIN);
    }
    else if(event_type == FdTriggerEvent::OUT_EVENT)
    {
        m_listen_events.events &= (~EPOLLOUT);
    }
}
}