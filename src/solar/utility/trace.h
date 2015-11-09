#pragma once

#include <functional>
#include "solar_dependencies/cppformat/format.h"

namespace solar {
	using trace_handler = std::function<void(const char*, int, bool, const char*)>;

	void set_trace_handler(trace_handler handler);
	void handle_trace(const char* file_name, int line_number, bool add_newline, const char* message);

	template <typename... Args>
	inline void trace(const char* file_name, int line_number, bool add_newline, const char* format, const Args& ... args) {
		handle_trace(file_name, line_number, add_newline, fmt::format(format, args...).c_str());
	}

	//overloading trace with no formatting so fmt::format isn't called on a message string that isn't expecting it
	inline void trace(const char* file_name, int line_number, bool add_newline, const char* message) {
		handle_trace(file_name, line_number, add_newline, message);
	}
}

#ifndef SOLAR__NO_TRACE

	#define TRACE(format, ...) solar::trace(__FILE__, __LINE__, true, format, __VA_ARGS__);

#else

	#define TRACE(format, ...)

#endif
