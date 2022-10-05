#include "utils/mkdir.h"

#include "utils/cleanup.h"

#include <string>
#include <fcntl.h>
#include <sys/stat.h>


namespace utils
{

//! make a new directory, does not throw error if dir already exists.
bool
mkdir_safe(const char* dirname)
{
    constexpr static auto mkdir_perms = S_IRWXU | S_IRWXG | S_IRWXO;

    auto res = mkdir(dirname, mkdir_perms);
    if (res == 0)
    {
        return false;
    }

    if (errno == EEXIST)
    {
        return true;
    }
    threrror(std::string("mkdir ") + std::string(dirname));
}

} /* utils */
