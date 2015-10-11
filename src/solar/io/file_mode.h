#pragma once

#include <ostream>

namespace solar {

	enum class file_mode {
		OPEN_READ,
		CREATE_WRITE,
		APPEND_WRITE
	};

	extern const char* to_string(file_mode mode);
	extern std::ostream& operator<<(std::ostream& os, file_mode mode);

}