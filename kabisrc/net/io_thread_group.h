#ifndef KABI_NET_IO_THREAD_GROUP_H
#define KABI_NET_IO_THREAD_GROUP_H
#include <vector>
#include "../include/log.h"
#include "io_thread.h"
namespace kabi
{
class ioThreadGroup
{
public:
//一般只有主线程需要调用组
    ioThreadGroup(int size);
    ~ioThreadGroup();
    void start();
    void join();
    ioThread* get_io_thread();

private:
    int m_size {0};
    int m_index {0};
    std::vector<ioThread*> m_io_thread_groups;
};

}
#endif //KABI_NET_IO_THREAD_GROUP_H