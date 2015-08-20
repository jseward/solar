#pragma once

#include <string>

namespace solar {

	void print_to_debug_output(const char* string);
	void print_to_debug_output(const std::string& string);

}

#ifdef _WIN32

	#define TRIGGER_BREAKPOINT __debugbreak();

#else

	#error("Uknown Platform")

#endif