#include "opengl_window_type.h"

#include "solar/utility/enum_helpers.h"
#include "solar/utility/assert.h"

namespace solar {

	const char* to_string(opengl_window_type e) {
		switch (e) {
			RETURN_ENUM_STRING(opengl_window_type, RESIZABLE_WINDOW);
			RETURN_ENUM_STRING(opengl_window_type, VIRTUAL_FULLSCREEN);
			END_ENUM_STRINGS(e);
		}
	}

}