#include "config.h"

#define READ_XML_NODE(name, parent)\
TiXmlElement* name##_node = parent->FirstChildElement(#name); \
if (!name##_node){\
    printf("Start kabi server error, failed to read node [%s]\n", #name);\
    exit(0);\
}\

#define READ_STR_FROM_XML_NODE(name, parent)\
TiXmlElement* name##_node = parent->FirstChildElement(#name);\
if(!name##_node || !name##_node->GetText()){\
    printf("Start kabi server error, failed to read config file %s\n", #name);\
    exit(0);\
}\
std::string name##_str = std::string(name##_node->GetText());\

namespace kabi
{
static config* g_config = nullptr;
config* config::get_global_config()
{
    return g_config;
}
void config::set_global_config(const char* xmlfile)
{
    if(g_config == nullptr)
        g_config = new config(xmlfile);
}
config::config(const char* xmlfile)
{
    TiXmlDocument* xml_document = new TiXmlDocument();
    bool rt = xml_document->LoadFile(xmlfile);
    if(!rt)
    {
        printf("Start kabi server error, failed to read config file %s, error info[%s]\n", xmlfile, xml_document->ErrorDesc());
        exit(0);
    }
    READ_XML_NODE(root, xml_document); //去xml里面找到root节点
    READ_XML_NODE(log, root_node);
    READ_STR_FROM_XML_NODE(log_level, log_node);
    m_log_level = log_level_str;
}

}