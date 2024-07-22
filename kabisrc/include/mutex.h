#ifndef KABI_KABISRC_INCLUDE_MUTEX_H
#define KABI_KABISRC_INCLUDE_MUTEX_H
#include <pthread.h>
#include <exception>
namespace kabi{
template<class T>
class scope_mutext
{
    //RAII机制：利用对象的析构函数做一些销毁的动作
public:
    scope_mutext(T& mutex): m_mutex(mutex)
    {
        m_mutex.lock();
        m_is_lock = true;
    }
    ~scope_mutext()
    {
        m_mutex.unlock();
        m_is_lock = false;
    }
    void lock()
    {
        if(!m_is_lock) 
        {
            m_mutex.lock();
            m_is_lock = true;
        }             
    }
    void unlock()
    {
        if(m_is_lock)
        {
            m_mutex.unlock();
            m_is_lock = false;
        }
    }
private:
    T& m_mutex;
    bool m_is_lock{false};
};
class mutex
{
public:
    mutex()
    {
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }

    }
    ~mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }
    pthread_mutex_t* get_mutex()
    {
        return &m_mutex;
    }
private:
    pthread_mutex_t m_mutex;
};
}
#endif //KABISRC_MUTEX_H