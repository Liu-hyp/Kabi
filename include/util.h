#ifndef KABI_INCLUDE_UTIL_H
#define KABI_INCLUDE_UTIL_H
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

namespace kabi{
pid_t get_pid();
pid_t get_thread_id();
int64_t get_now_ms();
}
#endif //KABI_INCLUDE_UTIL_H