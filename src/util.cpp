#include "util.h"

namespace kabi{
static int g_pid = 0;
static thread_local int t_thread_id = 0; //线程局部变量
pid_t get_pid()
{
    if(g_pid != 0)return g_pid;
    return getpid(); //调用系统方法
}
pid_t get_thread_id()
{
    if(t_thread_id != 0)return t_thread_id;
    return syscall(SYS_gettid);
}
int64_t get_now_ms()
{
    timeval val;
    gettimeofday(&val, nullptr);
    return val.tv_sec * 1000 + val.tv_usec / 1000;
}
}