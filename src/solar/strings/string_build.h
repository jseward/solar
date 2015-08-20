#pragma once

#include "solar_dependencies/cppformat/format.h"

namespace solar {

	inline std::string build_string(const char* format, fmt::ArgList args) {
		return fmt::format(format, args);
	}

	FMT_VARIADIC(std::string, build_string, const char *)

	template<typename Con>
	std::string build_container_string(const Con& con) {
		std::stringstream ss;
		ss << "[ ";
		bool is_first = true;
		for (const auto& e : con) {
			if (!is_first) {
				ss << " , ";
			}
			is_first = false;
			ss << e;
		}
		ss << " ]";
		return ss.str();
	}

}