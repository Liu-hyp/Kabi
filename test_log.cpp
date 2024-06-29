#include "kabi/common/config.h"
#include "kabi/common/log.h"
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
    kabi::config::set_global_config("../conf/kabi.xml");
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