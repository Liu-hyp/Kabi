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
public:
    static config* get_global_config();
    static void set_global_config(const char* xmlfile);
public:
    std::string m_log_level;

};
}
#endif //KABI_INCLUDE_CONFIG_H