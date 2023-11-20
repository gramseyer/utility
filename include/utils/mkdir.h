#pragma once

#include <string>
#include <filesystem>

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

[[maybe_unused]]
static void
clear_directory(const std::string& dirname)
{
	std::error_code ec;
	std::filesystem::remove_all({dirname}, ec);
	if (ec) {
		throw std::runtime_error("failed to clear directory");
	}
}

} /* utils */
