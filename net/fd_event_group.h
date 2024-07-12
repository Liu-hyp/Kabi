#ifndef KABI_NET_FD_EVENT_GROUP_H
#define KABI_NET_FD_EVENT_GROUP_H
#include <vector>
#include "fd_event.h"
#include "../include/mutex.h"
namespace kabi
{
class fdEventGroup
{
public:
    fdEventGroup(int size);
    ~fdEventGroup();
    fdEvent* get_fd_event(int fd);
public:
    static fdEventGroup* get_fd_event_group();
private:
    int m_size {0};
    std::vector<fdEvent*> m_fd_group;
    mutex m_mutex;
};
}
#endif //AKBI_NET_FD_EVENT_GROUP_H
