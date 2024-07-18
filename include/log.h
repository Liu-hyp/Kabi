#ifndef KABI_INCLUDE_LOG_H
#define KABI_INCLUDE_LOG_H

#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <memory>
#include <sys/time.h>
#include <mutex>
#include <semaphore.h>

#include "config.h"
#include "util.h"
#include "mutex.h"
#include "../net/timer_event.h"
#include <pthread.h>


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

// class logger
// {
// public:
//     typedef std::shared_ptr<logger> s_ptr;
//     logger(LOGLEVEL level): m_set_level(level) {};
//     void push_log(const std::string& msg);
//     void log();
//     LOGLEVEL get_log_level() const{
//         return m_set_level;
//     }
// public:
//     static logger* get_global_logger()
//     {        
//         return logger::instance;
//     }
//     static void init_global_logger()
//     {
//         if(config::get_global_config())
//         {
//             std::string tmp = config::get_global_config()->m_log_level;
//             LOGLEVEL level = string_to_log_level(tmp);
//             logger::instance = new logger(level);
//         }
//         logger::instance = new logger(LOGLEVEL::DEBUG);
//     }
// private:
//     LOGLEVEL m_set_level;
//     std::queue<std::string> m_buffer;
//     //std::mutex m_mutex;
//     mutex m_mutex;
//     static logger* instance;
// };
class asyncLogger {

public:
    typedef std::shared_ptr<asyncLogger> s_ptr;
    asyncLogger(const std::string& file_name, const std::string& file_path, int max_size);
    void stop();

    // 刷新到磁盘,写文件会先刷新到缓冲区
    void flush();

    void push_log_buffer(std::vector<std::string>& vec);


public:
    static void* Loop(void*);

public:
    pthread_t m_thread;

private:
  // m_file_path/m_file_name_yyyymmdd.0

    std::queue<std::vector<std::string>> m_buffer;

    std::string m_file_name;    // 日志输出文件名字
    std::string m_file_path;    // 日志输出路径
    int m_max_file_size {0};    // 日志单个文件最大大小, 单位为字节

    sem_t m_sempahore; //通知线程

    pthread_cond_t m_condition;  // 条件变量，必须和互斥锁一起用
    mutex m_mutex;

    std::string m_date;   // 当前打印日志的文件日期
    FILE* m_file_hanlder {NULL};   // 当前打开的日志文件句柄，防止每次打开的损耗

    bool m_reopen_flag {false}; //是否要重新打开日志，日志重命名或者滚动，新的日期就需要重新打开

    int m_no {0};   // 日志文件序号

    bool m_stop_flag {false};

};
//生产者-消费者实现异步日志，主线程将logger的buffer内容给异步logger的buffer
class logger {
public:
    typedef std::shared_ptr<logger> s_ptr;

    logger(LOGLEVEL level, int type = 1);

    void push_log(const std::string& msg);

    void push_app_log(const std::string& msg);

    void init();

    void log();

    LOGLEVEL get_log_level() const {
        return m_set_level;
    }

    asyncLogger::s_ptr get_async_app_logger() {
        return m_asnyc_app_logger;
    }

    asyncLogger::s_ptr get_async_logger() {
        return m_asnyc_logger;
    }

    void sync_loop();

    void flush();

public:
    static logger* get_global_logger();

    static void init_global_logger(int type = 1);

private:
    LOGLEVEL m_set_level;
    std::vector<std::string> m_buffer;

    std::vector<std::string> m_app_buffer;

    mutex m_mutex;

    mutex m_app_mutex;

    // m_file_path/m_file_name_yyyymmdd.1

    std::string m_file_name;    // 日志输出文件名字
    std::string m_file_path;    // 日志输出路径
    int m_max_file_size {0};    // 日志单个文件最大大小

    asyncLogger::s_ptr m_asnyc_logger;

    asyncLogger::s_ptr m_asnyc_app_logger;

    timerEvent::s_ptr m_timer_event;

    int m_type {0};

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
        kabi::logger::get_global_logger()->push_log((kabi::logEvent(kabi::LOGLEVEL::DEBUG)).to_string() \
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \   
    }\

#define INFOLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::INFO) \
{\
    kabi::logger::get_global_logger()->push_log((kabi::logEvent(kabi::LOGLEVEL::INFO)).to_string() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
}\

#define ERRORLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::ERROR) \
{\
    kabi::logger::get_global_logger()->push_log((kabi::logEvent(kabi::LOGLEVEL::ERROR)).to_string() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
}\

#define APPDEBUGLOG(str, ...)\
    if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::DEBUG) \
    {\
        kabi::logger::get_global_logger()->push_log((kabi::logEvent(kabi::LOGLEVEL::DEBUG)).to_string() \
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \   
    }\

#define APPINFOLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::INFO) \
{\
    kabi::logger::get_global_logger()->push_log((kabi::logEvent(kabi::LOGLEVEL::INFO)).to_string() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
}\

#define APPERRORLOG(str, ...)\
if(kabi::logger::get_global_logger()->get_log_level() <= kabi::LOGLEVEL::ERROR) \
{\
    kabi::logger::get_global_logger()->push_log((kabi::logEvent(kabi::LOGLEVEL::ERROR)).to_string() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + kabi::format_string(str, ##__VA_ARGS__) + "\n"); \
}\

#endif //KABI_INCLUDE_LOG_H