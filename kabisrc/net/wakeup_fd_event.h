#ifndef KABI_KABISRC_NET_WAKEUP_FD_EVENT_H
#define KABI_KABISRC_NET_WAKEUP_FD_EVENT_H
#include "fd_event.h"
#include <unistd.h>
namespace kabi
{
class wakeUpFdEvent : public fdEvent
{
public:
    wakeUpFdEvent(int fd);
    ~wakeUpFdEvent();
    void wakeup();
};
}
#endif //KABI_NET_WAKEUP_FD_EVENT_H