#pragma once

#include <stdint.h>
#include <string>

namespace solar {

	const char* bool_to_string(bool value);
	std::string hex_value_to_string(uint8_t hex_value);
	bool try_parse_hex_value_from_string(unsigned int& v, const char* s);
	unsigned int parse_hex_value_from_string(const char* s);

}