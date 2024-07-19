#include "io_thread_group.h"
namespace kabi
{
ioThreadGroup::ioThreadGroup(int size):m_size(size)
{
    m_io_thread_groups.resize(size);
    for(size_t i = 0; (int)i < size; ++i)
    {
        m_io_thread_groups[i] = new ioThread();
    }

}
ioThreadGroup::~ioThreadGroup()
{

}
void ioThreadGroup::start()
{
    for(size_t i = 0; i < m_io_thread_groups.size(); ++i)
    {
        m_io_thread_groups[i]->start();
    }
}
ioThread* ioThreadGroup::get_io_thread()
{
    if(m_index == m_io_thread_groups.size() || m_index == -1)
    {
        m_index = 0;
    }
    return m_io_thread_groups[m_index++];
}
void ioThreadGroup::join()
{
    for(size_t i = 0; i < m_io_thread_groups.size(); ++i)
    {
        m_io_thread_groups[i]->join();
    }
}
}