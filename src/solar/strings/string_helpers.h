#pragma once

#include <string>
#include <vector>

namespace solar {

	bool is_string_empty(const char* str);
	bool is_string_empty(const wchar_t* str);
	unsigned int get_string_length(const char* str);
	unsigned int get_string_length(const wchar_t* wstr);

	int string_replace_all(std::string& str, const char* find, const char* replace_with);

	int get_string_hash_code(const char* s);
	int get_string_hash_code(const std::string& s);

	bool are_strings_equal(const char* s0, const char* s1);

	std::vector<std::string> split_string(const char* s, const char* delimiters);

}