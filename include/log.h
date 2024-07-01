#ifndef KABI_INCLUDE_LOG_H
#define KABI_INCLUDE_LOG_H

#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <memory>
#include <sys/time.h>
#include <mutex>

#include "config.h"
#include "util.h"
#include "mutex.h"


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
    logger(LOGLEVEL level): m_set_level(level) {};
    void push_log(const std::string& msg);
    void log();
    LOGLEVEL get_log_level() const{
        return m_set_level;
    }
public:
    static logger* get_global_logger()
    {        
        return logger::instance;
    }
    static void init_global_logger()
    {
        if(config::get_global_config())
        {
            std::string tmp = config::get_global_config()->m_log_level;
            LOGLEVEL level = string_to_log_level(tmp);
            logger::instance = new logger(level);
        }
        logger::instance = new logger(LOGLEVEL::DEBUG);
    }
private:
    LOGLEVEL m_set_level;
    std::queue<std::string> m_buffer;
    //std::mutex m_mutex;
    mutex m_mutex;
    static logger* instance;
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

#define DEBUGLOG(str, ...)\
    if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::DEBUG) \
    {\
        kabi::logger::get_global_logger()->push_log((new kabi::logEvent(kabi::LOGLEVEL::DEBUG))->to_string() \
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
        kabi::logger::get_global_logger()->log(); \
    }\

#define INFOLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::INFO) \
{\
    kabi::logger::get_global_logger()->push_log((new kabi::logEvent(kabi::LOGLEVEL::INFO))->to_string() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
    kabi::logger::get_global_logger()->log(); \
}\

#define ERRORLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::ERROR) \
{\
    kabi::logger::get_global_logger()->push_log((new kabi::logEvent(kabi::LOGLEVEL::ERROR))->to_string() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
    kabi::logger::get_global_logger()->log(); \
}\

#endif //KABI_INCLUDE_LOG_H