#pragma once

static inline void SPINLOCK_PAUSE() {
#if __has_builtin(__builtin_ia32_pause)
    __builtin_ia32_pause();
}
#elif defined(__aarch64__)
    asm volatile ("yield");
#endif
}
