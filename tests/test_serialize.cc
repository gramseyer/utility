#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "utils/serialize_endian.h"

namespace utils {

TEST_CASE("serialize little endian", "[utils]")
{
	uint64_t value = 0xAAAA'BBBB'CCCC'DDDD;

	uint8_t data[8];

	write_unsigned_little_endian(data, value);

	REQUIRE(read_unsigned_little_endian<uint64_t>(data) == value);
}

}
