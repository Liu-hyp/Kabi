#include "fd_event_group.h"
#include "../include/log.h"
namespace kabi
{
static fdEventGroup* g_fd_event_group = NULL;
fdEventGroup* fdEventGroup::get_fd_event_group()
{
    if(g_fd_event_group != NULL)
    {
        return g_fd_event_group;
    }
    g_fd_event_group = new fdEventGroup(128);
    return g_fd_event_group;
}
fdEventGroup::fdEventGroup(int size) : m_size(size)
{
    for(int i = 0; i < m_size; ++i)
    {
        m_fd_group.push_back(new fdEvent(i));
    }

}
fdEventGroup::~fdEventGroup()
{
    for(int i = 0; i < m_size; ++i)
    {
        if(m_fd_group[i] != NULL)
        {
            delete m_fd_group[i];
            m_fd_group[i] = NULL;
        }
    }
}
fdEvent* fdEventGroup::get_fd_event(int fd)
{
    scope_mutext<mutex>locker(m_mutex);
    if(fd < m_fd_group.size())
    {
        return m_fd_group[fd];
    }
    int new_size = int(fd * 1.5);
    for(int i = m_fd_group.size(); i < new_size; ++i)
    {
        m_fd_group.push_back(new fdEvent(i));
    }
    return m_fd_group[fd];
}

}