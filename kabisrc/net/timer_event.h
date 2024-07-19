#ifndef KABI_NET_TIMER_EVENT_H
#define KABI_NET_TIMER_EVENT_H
#include <functional>
#include <memory>
namespace kabi{
class timerEvent
{
public:
    typedef std::shared_ptr<timerEvent> s_ptr;
    timerEvent(int interval, bool is_repeated, std::function<void()> cb);
    ~timerEvent();
    int64_t get_arrive_time() const{return m_arrive_time;}
    void set_cancel(bool value)
    {
        m_is_canceled = value;
    }
    bool is_cancled() const {return m_is_canceled;}
    bool is_repeated() const {return m_is_repeated;}
    std::function<void()> get_call_back()
    {
        return m_task;
    }
    void reset_arrive_time();
private:
    int64_t m_arrive_time; //ms
    int64_t m_interval; //ms
    bool m_is_repeated {false};
    bool m_is_canceled {false};
    std::function<void()> m_task;
};

}
#endif //KABI_NET_TIMER_EVENT_H