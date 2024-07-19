#ifndef KABI_NET_IO_THREAD_T
#define KABI_NET_IO_THREAD_T
#include "eventloop.h"
#include <semaphore.h>
namespace kabi
{
class ioThread
{
public:
    ioThread();
    ~ioThread();
    eventloop* get_event_loop();
    void start();
    void join();
public:
    static void* Main(void* arg);
private:
    pid_t m_thread_id {-1}; //线程号
    pthread_t m_thread {0}; //线程句柄
    eventloop* m_event_loop {nullptr}; //当前io的loop对象
    sem_t m_init_semaphore;
    sem_t m_start_semaphore;
};
}
#endif //KABI_NET_IO_THREAD_T
