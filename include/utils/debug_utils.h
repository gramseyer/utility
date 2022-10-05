#pragma once

/*! \file debug_utils.h 
Miscellaneous debug utility functions.
*/

#include <sstream>
#include <iomanip>

namespace utils 
{

//! Convert a byte array to a hex string.
static std::string array_to_str(const unsigned char* array, const int len) {
	std::stringstream s;
	s.fill('0');
	for (int i = 0; i < len; i++) {
		s<< std::setw(2) << std::hex << (unsigned short)array[i];
	}
	return s.str();
}

template<typename ArrayLike>
static std::string array_to_str(const ArrayLike& array)
{
	return array_to_str(array.data(), array.size());
}

} /* utils */

