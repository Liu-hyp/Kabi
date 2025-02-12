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
    if (g_config == NULL) 
    {
        if (xmlfile != NULL) 
        {
            g_config = new config(xmlfile);
        } else {
            g_config = new config();
        }
    }
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
    READ_XML_NODE(server, root_node)
    READ_STR_FROM_XML_NODE(log_level, log_node);
    READ_STR_FROM_XML_NODE(log_file_name, log_node);
    READ_STR_FROM_XML_NODE(log_file_path, log_node);
    READ_STR_FROM_XML_NODE(log_max_file_size, log_node);
    READ_STR_FROM_XML_NODE(log_sync_interval, log_node);
    m_log_level = log_level_str;
    m_log_file_name = log_file_name_str;
    m_log_file_path = log_file_path_str;
    m_log_max_file_size = std::atoi(log_max_file_size_str.c_str()) ;
    m_log_sync_inteval = std::atoi(log_sync_interval_str.c_str());
    m_log_level = log_level_str;
    printf("LOG -- CONFIG LEVEL[%s], FILE_NAME[%s],FILE_PATH[%s] MAX_FILE_SIZE[%d B], SYNC_INTEVAL[%d ms]\n", 
    m_log_level.c_str(), m_log_file_name.c_str(), m_log_file_path.c_str(), m_log_max_file_size, m_log_sync_inteval);
    READ_STR_FROM_XML_NODE(port, server_node);
    READ_STR_FROM_XML_NODE(io_threads, server_node);
    m_port = std::atoi(port_str.c_str());
    m_io_threads = std::atoi(io_threads_str.c_str());
    printf("SERVER -- PORT[%d], IO_THREADS\n", m_port, m_io_threads);
}
config::config()
{
    m_log_level = "DEBUG";
}

}