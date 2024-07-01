#include "log.h"

namespace kabi{
logger* logger::instance = nullptr;
std::string log_level_to_string(kabi::LOGLEVEL level)
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
LOGLEVEL string_to_log_level(const std::string& log_level)
{
    if(log_level == "DEBUG")
    {
        return LOGLEVEL::DEBUG;
    }
    else if(log_level == "INFO")
    {
        return LOGLEVEL::INFO;
    }
    else if(log_level == "ERROR")
    {
        return LOGLEVEL::ERROR;
    }
    else
    {
        return LOGLEVEL::UNKNOWN;
    }

}

std::string logEvent::to_string()
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

void logger::push_log(const std::string& msg)
{
    //std::scoped_lock<std::mutex>locker(m_mutex);
    kabi::scope_mutext<kabi::mutex>locker(m_mutex);
    m_buffer.push(msg);
}

void logger::log()
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

}
