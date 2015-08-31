#include "string_helpers.h"

#include <memory>
#include "solar/utility/verify.h"

namespace solar {

	bool is_string_empty(const char* str) {
		return ::strlen(str) == 0;
	}

	bool is_string_empty(const wchar_t* wstr) {
		return ::wcslen(wstr) == 0;
	}

	unsigned int get_string_length(const char* str) {
		return ::strlen(str);
	}

	unsigned int get_string_length(const wchar_t* wstr) {
		return ::wcslen(wstr);
	}

	int string_replace_all(std::string& str, const char* find, const char* replace_with) {
		//http://stackoverflow.com/questions/1494399/how-do-i-search-find-and-replace-in-a-standard-string
		int count = 0;
		size_t pos = 0;
		while ((pos = str.find(find, pos)) != std::string::npos) {
			str.replace(pos, get_string_length(find), replace_with);
			pos += get_string_length(replace_with);
			count += 1;
		}
		return count;
	}

	int get_string_hash_code(const char* s) {
		//Hash a string using the method of Fowler, Noll, and Vo, given in
		//http://www.isthe.com/chongo/tech/comp/fnv.
		//http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200209/homework10/hashfuncs.cc
		unsigned int hash_code = 2166136261u;
		int i = 0;
		while (s[i] != '\0') {
			hash_code = hash_code * 6777619u ^ s[i];
			i++;
		}
		return hash_code;
	}

	int get_string_hash_code(const std::string& s) {
		return get_string_hash_code(s.c_str());
	}

	bool are_strings_equal(const char* s0, const char* s1) {
		return (::strcmp(s0, s1) == 0);
	}

	std::vector<std::string> split_string(const char* s, const char* delimiters) {
		
		int buffer_size = get_string_length(s) + 1;
		auto buffer = std::unique_ptr<char[]>(new char[buffer_size]);
		::strcpy_s(buffer.get(), buffer_size, s);

		std::vector<std::string> results;
		
		char* context = nullptr;
		char* token = ::strtok_s(buffer.get(), delimiters, &context);
		while (token != nullptr) {
			results.push_back(token);
			token = ::strtok_s(nullptr, delimiters, &context);
		}

		return results;
	}

	bool does_string_start_with(const char* s, const char* start_with) {
		unsigned int s_len = get_string_length(s);
		unsigned int start_with_len = get_string_length(start_with);

		if (start_with_len > s_len ) {
			return false;
		}

		for (unsigned int i = 0; i < start_with_len; ++i) {
			if (s[i] != start_with[i]) {
				return false;
			}
		}

		return true;
	}

}