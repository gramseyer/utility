#pragma once

#include <string>

namespace utils
{

bool 
mkdir_safe(const char* dirname);

[[maybe_unused]]
static bool
mkdir_safe(const std::string& dirname)
{
	return mkdir_safe(dirname.c_str());
}

} /* utils */
