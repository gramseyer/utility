#pragma once

#include <string>

namespace utils
{

bool 
mkdir_safe(const char* dirname);

bool
mkdir_safe(const std::string& dirname)
{
	return mkdir_safe(dirname.c_str());
}

} /* utils */
