#include "wakeup_fd_event.h"
#include "../include/log.h"
namespace kabi
{
wakeUpFdEvent::wakeUpFdEvent(int fd) : fdEvent(fd)
{

}
wakeUpFdEvent::~wakeUpFdEvent()
{

}
void wakeUpFdEvent::wakeup()
{
    char buf[8] = {'a'};
    int rt = write(m_fd, buf, 8);
    if(rt != 8)
    {
        ERRORLOG("failed tp wake up fd less than 8 bytes, fd[%d]", m_fd);
    }
}
}
