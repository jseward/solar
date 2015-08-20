#include "string_convert.h"
#include "solar/utility/verify.h"

namespace solar {

	const char* bool_to_string(bool value) {
		return value ? "true" : "false";
	}

	std::string hex_value_to_string(uint8_t hex_value) {
		char s[3];
		VERIFY(::sprintf_s(s, 3, "%02x", hex_value) == 2);
		return s;
	}

	bool try_parse_hex_value_from_string(unsigned int& v, const char* s) {
		int scanf_result = ::sscanf_s(s, "%x", &v);
		return (scanf_result == 1);
	}

	unsigned int parse_hex_value_from_string(const char* s) {
		unsigned int v = 0;
		VERIFY(try_parse_hex_value_from_string(v, s));
		return v;
	}

}