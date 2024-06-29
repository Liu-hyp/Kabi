#ifndef KABI_COMMON_CONFIG_H
#define KABI_COMMON_CONFIG_H
#include <map>
#include <string>
#include <tinyxml/tinyxml.h>
namespace kabi{
class config
{
public:
    config(const char* xmlfile);
public:
    static config* get_global_config();
    static void set_global_config(const char* xmlfile);
public:
    std::string m_log_level;

};
}
#endif //KABI_COMMON_CONFIG_H