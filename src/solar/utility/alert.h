#pragma once

#include <functional>
#include "debugging.h"
#include "solar_dependencies/cppformat/format.h"

namespace solar {
	using alert_handler = std::function<bool(const char*, int, const char*)>;

	void set_alert_handler(alert_handler handler);
	bool handle_alert(const char* file_name, int line_number, const char* message);

	template <typename... Args>
	inline bool alert(const char* file_name, int line_number, const char* format, const Args& ... args) {
		return handle_alert(file_name, line_number, fmt::format(format, args...).c_str());
	}

	//overloading alert with no formatting so fmt::format isn't called on a message string that isn't expecting it
	inline bool alert(const char* file_name, int line_number, const char* message) {
		return handle_alert(file_name, line_number, message);
	}
}

#ifndef SOLAR__NO_ALERT

	#define ALERT(format, ...) \
		if (solar::alert(__FILE__, __LINE__, format, __VA_ARGS__)) {\
			TRIGGER_BREAKPOINT; \
		}

#else

	#define ALERT(format, ...)

#endif