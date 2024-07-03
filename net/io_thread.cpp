#include "io_thread.h"
#include <assert.h>
#include "../include/log.h"
namespace kabi{
ioThread::ioThread()
{
    int rt = sem_init(&m_init_semaphore, 0, 0);
    assert(rt == 0);
    int ret = sem_init(&m_start_semaphore, 0, 0);
    assert(ret == 0);
    pthread_create(&m_thread, NULL, &ioThread::Main, this); //传this指针可以把对象再取出来
    //需要做同步，在这里等待新线程执行完main函数eventloop的前置
    sem_wait(&m_init_semaphore);
    DEBUGLOG("success create ioThread %d", m_thread_id);
}
ioThread::~ioThread()
{
    m_event_loop->stop();
    sem_destroy(&m_init_semaphore);
    sem_destroy(&m_start_semaphore);
    pthread_join(m_thread, NULL);
    if(m_event_loop){
        delete m_event_loop;
        m_event_loop = nullptr;
    }
}
void* ioThread::Main(void* arg)
{
    ioThread* thread = static_cast<ioThread*>(arg);
    thread->m_event_loop = new eventloop();
    thread->m_thread_id = get_thread_id();
    //唤醒等待的线程-信号量
    sem_post(&thread->m_init_semaphore);
    //让IO线程等待，直到我们主动启动
    DEBUGLOG("ioThread %d wait loop", thread->m_thread_id);
    sem_wait(&thread->m_start_semaphore);
    DEBUGLOG("ioThread %d start loop", thread->m_thread_id);
    thread->m_event_loop->loop();
    DEBUGLOG("ioThread %d end loop", thread->m_thread_id);
    return NULL;
}
eventloop* ioThread::get_event_loop()
{
    return m_event_loop;
}
void ioThread::start()
{
    DEBUGLOG("now invoke ioThread %d", m_thread_id);
    sem_post(&m_start_semaphore);
}
void ioThread::join()
{
    pthread_join(m_thread, NULL);
}

}
