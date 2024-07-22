#ifndef KABI_KABISRC_INCLUDE_UTIL_H
#define KABI_KABISRC_INCLUDE_UTIL_H
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <memory>

namespace kabi{
pid_t get_pid();
pid_t get_thread_id();
int64_t get_now_ms();
int32_t get_int32_from_net_byte(const char* buffer);
}
#endif //KABI_INCLUDE_UTIL_H