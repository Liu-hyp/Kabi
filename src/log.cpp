#include "log.h"
#include "../net/eventloop.h"
#include "runtime.h"

namespace kabi{
//logger* logger::instance = nullptr;
static logger* g_logger = NULL;
logger* logger::get_global_logger() {
  return g_logger;
}
logger::logger(LOGLEVEL level, int type /*=1*/) : m_set_level(level), m_type(type) {

    if (m_type == 0) {
        return;
    }
    m_asnyc_logger = std::make_shared<asyncLogger>(
        config::get_global_config()->m_log_file_name + "_rpc",
        config::get_global_config()->m_log_file_path,
        config::get_global_config()->m_log_max_file_size);
    
    m_asnyc_app_logger = std::make_shared<asyncLogger>(
        config::get_global_config()->m_log_file_name + "_app",
        config::get_global_config()->m_log_file_path,
        config::get_global_config()->m_log_max_file_size);
}
void logger::init()
{
    m_timer_event = std::make_shared<timerEvent>(config::get_global_config()->m_log_sync_inteval, true, 
        std::bind(&logger::sync_loop, this));
    eventloop::get_current_event_loop()->add_timer_event(m_timer_event);
}
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
    
    //获取当前线程处理的请求的msgid
    std::string msgid = runTime::GetRunTime()->m_msgid;
    std::string method_name = runTime::GetRunTime()->m_method_name;
    if(!msgid.empty())
    {
        ss << "[" << msgid << "]\t";
    }
    if(!method_name.empty())
    {
        ss << "[" << method_name << "]\t";
    }
    return ss.str();
}

void logger::push_log(const std::string& msg)
{
    //std::scoped_lock<std::mutex>locker(m_mutex);
    //kabi::scope_mutext<kabi::mutex>locker(m_mutex);
    if (m_type == 0) {
        printf((msg + "\n").c_str());
        return;
    }
    scope_mutext<mutex> locker(m_mutex);
    m_buffer.push_back(msg);
    locker.unlock();
}
void logger::push_app_log(const std::string& msg)
{
    if (m_type == 0) {
        printf((msg + "\n").c_str());
        return;
    }
    scope_mutext<mutex> locker_app(m_app_mutex);
    m_app_buffer.push_back(msg);
    locker_app.unlock();
}

void logger::sync_loop()
{
    // 同步 m_buffer 到 async_logger 的buffer队尾
    // printf("sync to async logger\n");
    std::vector<std::string> tmp_vec;
    scope_mutext<mutex> locker(m_mutex);
    tmp_vec.swap(m_buffer);
    locker.unlock();

    if (!tmp_vec.empty()) 
    {
        m_asnyc_logger->push_log_buffer(tmp_vec);
    }
    tmp_vec.clear();

    // 同步 m_app_buffer 到 app_async_logger 的buffer队尾
    std::vector<std::string> tmp_vec2;
    scope_mutext<mutex> locker_app(m_app_mutex);
    tmp_vec2.swap(m_app_buffer);
    locker_app.unlock();

    if (!tmp_vec2.empty()) 
    {
        m_asnyc_app_logger->push_log_buffer(tmp_vec2);
    }
    tmp_vec2.clear();
}
void logger::init_global_logger(int type/* = 1*/)
{
    LOGLEVEL global_log_level = string_to_log_level(config::get_global_config()->m_log_level);
    printf("Init log level [%s]\n", log_level_to_string(global_log_level).c_str());
    g_logger = new logger(global_log_level, type);
    g_logger->init();
}
void logger::log()
{    
    // kabi::scope_mutext<kabi::mutex>locker(m_mutex);
    // std::queue<std::string>tmp;
    // m_buffer.swap(tmp);
    // locker.unlock();

    // while(!tmp.empty())
    // {
    //     std::string msg = tmp.front();
    //     tmp.pop();
    //     printf(msg.c_str()); //兼容C
    // }

}
asyncLogger::asyncLogger(const std::string& file_name, const std::string& file_path, int max_size)
    : m_file_name(file_name), m_file_path(file_path), m_max_file_size(max_size)
{
    sem_init(&m_sempahore, 0, 0);
    pthread_create(&m_thread, NULL, &asyncLogger::Loop, this);
    pthread_cond_init(&m_condition, NULL);
    sem_wait(&m_sempahore); //等待新线程去执行loop函数
}
void* asyncLogger::Loop(void* arg)
{
    
    //将buffer里面的全部数据打印到文件中，然后线程睡眠，直到有新的数据再重复这个过程
    asyncLogger* logger = reinterpret_cast<asyncLogger*>(arg); 
    sem_post(&logger->m_sempahore);
    while(1)
    {
        scope_mutext<mutex>locker(logger->m_mutex);
        while(logger->m_buffer.empty()) 
        {
            // printf("begin pthread_cond_wait back \n");
            pthread_cond_wait(&(logger->m_condition), logger->m_mutex.get_mutex());
        }
        std::vector<std::string> tmp;
        tmp.swap(logger->m_buffer.front());
        logger->m_buffer.pop();
        locker.unlock();

        timeval now;
        gettimeofday(&now, NULL);
        struct tm now_time;
        localtime_r(&(now.tv_sec), &now_time);   
        const char* format = "%Y%m%d";
        char date[32];
        strftime(date, sizeof(date), format, &now_time);

        if (std::string(date) != logger->m_date) //不是上次的日期，跨天
        {
            logger->m_no = 0;
            logger->m_reopen_flag = true;
            logger->m_date = std::string(date);
        }
        if (logger->m_file_hanlder == NULL) //没有打开过文件，是新文件
        {
            logger->m_reopen_flag = true;
        }
        std::stringstream ss;
        ss << logger->m_file_path << logger->m_file_name << "_"
            << std::string(date) << "_log.";
        std::string log_file_name = ss.str() + std::to_string(logger->m_no);

        if (logger->m_reopen_flag) 
        {
            if (logger->m_file_hanlder) 
            {
                fclose(logger->m_file_hanlder);
            }
            logger->m_file_hanlder = fopen(log_file_name.c_str(), "a");
            logger->m_reopen_flag = false;
        }
        //当前文件字节数比最大的还大
        if (ftell(logger->m_file_hanlder) > logger->m_max_file_size) 
        {
            fclose(logger->m_file_hanlder);
            log_file_name = ss.str() + std::to_string(logger->m_no++);
            logger->m_file_hanlder = fopen(log_file_name.c_str(), "a");
            logger->m_reopen_flag = false;
        }
        for (auto& i : tmp) 
        {
            if (!i.empty()) 
            {
                fwrite(i.c_str(), 1, i.length(), logger->m_file_hanlder);
            }
        }
        fflush(logger->m_file_hanlder); //日志才会真正落到磁盘

        if (logger->m_stop_flag) 
        {
            return NULL;
        }   
        
    }

    return NULL;
}
void asyncLogger::stop()
{
    m_stop_flag = true;
}
void asyncLogger::flush()
{
    if (m_file_hanlder) 
    {
        fflush(m_file_hanlder);
    }
}
void asyncLogger::push_log_buffer(std::vector<std::string>& vec)
{
    scope_mutext<mutex> locker(m_mutex);
    m_buffer.push(vec);
    pthread_cond_signal(&m_condition);

    locker.unlock();
    pthread_cond_signal(&m_condition);
    // 这时候需要唤醒异步日志线程
    // printf("pthread_cond_signal\n");
}



}
