#pragma once

//x86
#if __has_builtin(__builtin_ia32_pause)
#define SPINLOCK_PAUSE __builtin_ia32_pause
#elif __defined(__arm)
#define SPINLOCK_PAUSE __yield
#endif