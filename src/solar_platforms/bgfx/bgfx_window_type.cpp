#include "bgfx_window_type.h"

#include "solar/utility/enum_helpers.h"
#include "solar/utility/assert.h"

namespace solar {

	const char* to_string(bgfx_window_type e) {
		switch (e) {
			RETURN_ENUM_STRING(bgfx_window_type, RESIZABLE_WINDOW);
			RETURN_ENUM_STRING(bgfx_window_type, VIRTUAL_FULLSCREEN);
			END_ENUM_STRINGS(e);
		}
	}

}