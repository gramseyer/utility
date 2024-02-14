#pragma once

/*! \file spinlock.h

Implements a simple spinlock, based on
https://rigtorp.se/spinlock/
*/

#include <atomic>

#include "utils/non_movable.h"
#include "utils/compat.h"

namespace utils {

//! Mutex based on a spinlock
class SpinMutex
{
    mutable std::atomic<bool> flag;

  public:
    void lock() const
    {
        while (true) {
            bool res = flag.load(std::memory_order_relaxed);

            if (!res) {
                if (!flag.exchange(true, std::memory_order_acquire)) {
                    return;
                }
            }
            SPINLOCK_PAUSE();
        }
    }

    void unlock() const { flag.store(false, std::memory_order_release); }
};

//! Automatically unlocking wrapper around SpinMutex
class SpinLockGuard : public utils::NonMovableOrCopyable
{
    const SpinMutex& mtx;

  public:
    SpinLockGuard(const SpinMutex& mtx)
        : mtx(mtx)
    {
        mtx.lock();
    }

    ~SpinLockGuard() { mtx.unlock(); }
};

//! Automatically unlocking unique lock wrapper around SpinMutex
class SpinUniqueLock : public utils::NonMovableOrCopyable
{
    const SpinMutex* mtx;
    bool locked = false;

  public:
    SpinUniqueLock(const SpinMutex& mtx_)
        : mtx(&mtx_)
    {
        mtx->lock();
        locked = true;
    }

    SpinUniqueLock& operator=(SpinUniqueLock&& other)
    {
        if (locked) {
            mtx->unlock();
            locked = false;
        }
        mtx = other.mtx;
        locked = other.locked;

        other.mtx = nullptr;
        other.locked = false;

        return *this;
    }

    ~SpinUniqueLock()
    {
        if (locked) {
            mtx->unlock();
        }
    }
};

} // namespace utils
