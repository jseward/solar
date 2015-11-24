#include "file_mode.h"

#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* to_string(file_mode mode) {
		switch (mode) {
			RETURN_ENUM_STRING(file_mode, OPEN_READ);
			RETURN_ENUM_STRING(file_mode, CREATE_WRITE);
			RETURN_ENUM_STRING(file_mode, APPEND_WRITE);
			END_ENUM_STRINGS(mode);
		}
	};

	std::ostream& operator<<(std::ostream& os, file_mode mode) {
		return os << to_string(mode);
	}

}