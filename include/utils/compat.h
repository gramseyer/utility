#pragma once

#include <utility>

static inline void SPINLOCK_PAUSE()
{
    #if __has_builtin(__builtin_ia32_pause)
        __builtin_ia32_pause();
    #elif defined(__aarch64__)
        asm volatile ("yield");
    #endif
}

#if __cpp_lib_unreachable > 202202L
void unreachable() {
    std::unreachable();
}
#else
void unreachable() {
    perror("unreachable!");
    std::abort();
}
#endif

