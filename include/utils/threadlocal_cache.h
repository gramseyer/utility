#pragma once

/*! \file threadlocal_cache.h

Maintain a concurrently-accessible threadlocal storage,
without explicitly using pthread tls keys.
*/

#include <array>
#include <atomic>
#include <cstdint>
#include <optional>

#include "mtt/utils/non_movable.h"

namespace utils {

//! Stores a unique thread_local identifier (uint32_t).
//! Call get() to access id.
class ThreadlocalIdentifier : private NonMovableOrCopyable
{
    static uint32_t initialize() {
        uint32_t val = _tl_initializer.fetch_add(1, std::memory_order_relaxed);

        if (val == UINT32_MAX)
        {
            // initializer has overflowed
            throw std::runtime_error("overflow ThreadlocalIdentifier");
        }
        return val;
    }

    inline static std::atomic<uint32_t> _tl_initializer = 0;
    inline static thread_local uint32_t tid = initialize();

  public:
    static uint32_t get() { return tid; }
};

/*! Threadlocal object cache.

Using ThreadlocalIdentifier is safe in any config, since each thread's id
is unique.  Downside is that more threads running could consume identifiers,
and so at some point, we'll need to boost CACHE_SIZE.

An alternative is TBB's tbb::this_task_arena::current_thread_index,
(available in <tbb/task_arena.h>)
which keeps ids in a fixed (small) range.

The downside is that migrating this cache across arenas would break
any exported references or cause concurrent access to non-threadsafe objects.
This would be dangerous (easy to forget this restriction). YMMV.
*/

template<typename ValueType, uint32_t CACHE_SIZE = 256>
class ThreadlocalCache : private NonMovableOrCopyable
{

    std::array<std::optional<ValueType>, CACHE_SIZE> objects;

  public:
    using cache_array_t = std::array<std::optional<ValueType>, CACHE_SIZE>;

    ThreadlocalCache()
        : objects()
    {}

    //! Get current thread's cached object.
    //! Input constructor arguments for cached object, in the case
    //! that this is the first access (and so thread's cache is empty).
    template<typename... ctor_args>
    ValueType& get(ctor_args&... args)
    {
        uint32_t idx = ThreadlocalIdentifier::get();
        if (idx >= CACHE_SIZE) {
            throw std::runtime_error("invalid tlcache access!");
        }
        if (!objects[idx]) {
            objects[idx].emplace(args...);
        }
        return *objects[idx];
    }

    //! At times it is useful to export the list of cached objects
    //! all at once.
    std::array<std::optional<ValueType>, CACHE_SIZE>& get_objects()
    {
        return objects;
    }

    void clear()
    {
        for (uint32_t i = 0; i < CACHE_SIZE; i++) {
            objects[i] = std::nullopt;
        }
    }
};

} /* namespace utils */
