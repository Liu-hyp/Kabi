#ifndef KABI_INCLUDE_CONFIG_H
#define KABI_INCLUDE_CONFIG_H
#include <map>
#include <string>
#include <tinyxml/tinyxml.h>
namespace kabi{
class config
{
public:
    config(const char* xmlfile);
    config();
public:
    static config* get_global_config();
    static void set_global_config(const char* xmlfile);
public:
    std::string m_log_level;
    std::string m_log_file_name;
    std::string m_log_file_path;
    int m_log_max_file_size {0};
    int m_log_sync_inteval {0};   // 日志同步间隔，ms
    int m_port {0};
    int m_io_threads {0};

};
}
#endif //KABI_INCLUDE_CONFIG_H