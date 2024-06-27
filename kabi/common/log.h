#ifndef KABI_COMMON_LOG_H
#define KABI_COMMON_LOG_H

#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <memory>
#include <sys/time.h>
#include <mutex>

#include "kabi/common/config.h"
#include "kabi/common/util.h"
#include "kabi/common/mutex.h"

namespace kabi{
template<typename... Args>
std::string format_string(const char* str, Args&&... args)
{
    int size = snprintf(nullptr, 0, str, args...); //不需要存结果，只需要size
    std::string result;
    if(size > 0)
    {
        result.resize(size);
        snprintf(&result[0], size + 1, str, args...);
    }
    return result;
}

enum class LOGLEVEL
{
    UNKNOWN= 0,
    DEBUG = 1,
    INFO = 2,
    ERROR = 3
};
std::string log_level_to_string(LOGLEVEL level);
LOGLEVEL string_to_log_level(const std::string& log_level);

class logger
{
public:
    typedef std::shared_ptr<logger> s_ptr;
    logger(LOGLEVEL level): m_set_level(level){};
    void push_log(const std::string& msg);
    void log();
    LOGLEVEL get_log_level() const{
        return m_set_level;
    }
public:
    static logger* get_global_logger();
    static void init_global_logger();
private:
    LOGLEVEL m_set_level;
    std::queue<std::string> m_buffer;
    //std::mutex m_mutex;
    kabi::mutex m_mutex;
};

class logEvent{
public:
    logEvent(LOGLEVEL level):m_level(level){}
    std::string get_file_name() const 
    {
        return m_file_name;
    }
    logEvent get_log_event() const
    {
        return m_level;
    }
    std::string to_string();

private:
    std::string m_file_name; //文件名
    int32_t m_file_line; //行号
    int32_t m_pid; //进程号
    int32_t m_thread_id; //线程号
    LOGLEVEL m_level; //日志级别
};
}

#endif //KABI_COMMON_LOG_H