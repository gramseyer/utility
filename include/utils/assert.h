#pragma once

#include <cstdio>
#include <cstring>
#include <stdexcept>

namespace utils {

// TBB silently suppresses most thrown exceptions,
// rethrowing but without the error message.
// This prints the assert message, flushes to stdout,
// and then throws the exception 
[[maybe_unused]] 
static void
print_assert(bool query, const char* msg)
{
    if (!query) {
        std::printf("%s\n", msg);
        std::fflush(stdout);
        throw std::runtime_error(msg);
    }
}

[[maybe_unused]]
static void
print_assert(bool query, const std::string& str)
{
    print_assert(query, str.c_str());
}

} // namespace utils
