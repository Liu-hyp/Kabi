#include "kabi/common/log.h"
using namespace kabi;
using namespace std;
std::string kabi::log_level_to_string(kabi::LOGLEVEL level)
{
    switch (level)
    {
    case kabi::LOGLEVEL::DEBUG:
        return "DEBUG";
    case kabi::LOGLEVEL::INFO:
        return "INFO";
    case kabi::LOGLEVEL::ERROR:
        return "ERROR";   
    default:
        return "UNKNOWN";
    }
}
kabi::LOGLEVEL kabi::string_to_log_level(const std::string& log_level)
{
    if(log_level == "DEBUG")
    {
        return kabi::LOGLEVEL::DEBUG;
    }
    else if(log_level == "INFO")
    {
        return kabi::LOGLEVEL::INFO;
    }
    else if(log_level == "ERROR")
    {
        return kabi::LOGLEVEL::ERROR;
    }
    else
    {
        return kabi::LOGLEVEL::UNKNOWN;
    }

}

std::string kabi::logEvent::to_string()
{
    struct timeval now_time;
    gettimeofday(&now_time, nullptr);

    struct tm now_time_t;
    localtime_r(&(now_time.tv_sec), &now_time_t);

    char buf[128];
    strftime(&buf[0], 128, "%y-%m-%d %H:%M:%S", &now_time_t);
    std::string time_str(buf);
    int ms = now_time.tv_usec / 1000;
    time_str = time_str + "." + std::to_string(ms);

    m_pid = kabi::get_pid();
    m_thread_id = kabi::get_thread_id();

    std::stringstream ss;
    ss << "[" << kabi::log_level_to_string(m_level) << "]\t"
        << "[" << time_str << "]\t"
        << "[" << m_pid << ":" << m_thread_id << "]\t";
    return ss.str();
}

void kabi::logger::push_log(const std::string& msg)
{
    //std::scoped_lock<std::mutex>locker(m_mutex);
    kabi::scope_mutext<kabi::mutex>locker(m_mutex);
    m_buffer.push(msg);
}

void kabi::logger::log()
{    
    kabi::scope_mutext<kabi::mutex>locker(m_mutex);
    std::queue<std::string>tmp;
    m_buffer.swap(tmp);
    locker.unlock();

    while(!tmp.empty())
    {
        std::string msg = tmp.front();
        tmp.pop();
        printf(msg.c_str()); //兼容C
    }

}
#define DEBUGLOG(str, ...)\
    if(kabi::logger::get_global_logger()->get_log_level <= kabi::LOGLEVEL::DEBUG) \
    {\
        kabi::logger::get_global_logger()->push_log((new kabi::logEvent(kabi::LOGLEVEL::DEBUG))->to_string() \
        + "[" + std:;string(__FINE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::formatstring(str, ##__VA_ARGS__) + "\n"); \
        kabi::logger::get_global_logger()->log(); \
    }\

#define INFOLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level <= kabi::LOGLEVEL::INFO) \
{\
    kabi::logger::get_global_logger()->push_log((new kabi::logEvent(kabi::LOGLEVEL::INFO))->to_string() \
    + "[" + std:;string(__FINE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::formatstring(str, ##__VA_ARGS__) + "\n"); \
    kabi::logger::get_global_logger()->log(); \
}\

#define ERRORLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level <= kabi::LOGLEVEL::ERROR) \
{\
    kabi::logger::get_global_logger()->push_log((new kabi::logEvent(kabi::LOGLEVEL::ERROR))->to_string() \
    + "[" + std:;string(__FINE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::formatstring(str, ##__VA_ARGS__) + "\n"); \
    kabi::logger::get_global_logger()->log(); \
}\