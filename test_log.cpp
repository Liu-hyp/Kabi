#include "include/config.h"
#include "include/log.h"
#include <thread>

void* fun(void*)
{
    int i = 20;
    while(i--)
    {
        DEBUGLOG("debug this is thread in %s", "fun");
        INFOLOG("info this is thread in %s", "fun");
    }
    return nullptr;
}

int main()
{
    std::string xmlfile = "/mnt/hgfs/Share/Kabi/kabi.xml";
    kabi::config::set_global_config(xmlfile.c_str());
    kabi::logger::init_global_logger();
    pthread_t thread;
    pthread_create(&thread, NULL, &fun, NULL);
    int i = 20;
    while(i--)
    {
        DEBUGLOG("test debug log %s", "11");
        INFOLOG("test info log %s", "11");
    }
    pthread_join(thread, NULL);
    return 0;
}