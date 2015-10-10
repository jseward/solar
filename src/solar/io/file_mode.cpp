#include "file_mode.h"

#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* file_mode_details::get_string(file_mode e) {
		switch (e) {
			RETURN_ENUM_STRING(file_mode, OPEN_READ);
			RETURN_ENUM_STRING(file_mode, CREATE_WRITE);
			RETURN_ENUM_STRING(file_mode, APPEND_WRITE);
			END_ENUM_STRINGS(e);
		}
	};

}