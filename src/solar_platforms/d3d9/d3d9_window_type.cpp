#include "d3d9_window_type.h"

#include "solar/utility/enum_helpers.h"
#include "solar/utility/assert.h"

namespace solar {

	const char* d3d9_window_type_details::get_string(d3d9_window_type e) {
		switch (e) {
			RETURN_ENUM_STRING(d3d9_window_type, RESIZABLE_WINDOW);
			RETURN_ENUM_STRING(d3d9_window_type, VIRTUAL_FULLSCREEN);
			RETURN_ENUM_STRING(d3d9_window_type, EXCLUSIVE_FULLSCREEN);
			RETURN_ENUM_STRING(d3d9_window_type, CHILD_WINDOW);
			END_ENUM_STRINGS();
		}
	}

	bool d3d9_window_type_details::is_device_windowed(d3d9_window_type e) {
		switch (e) {
			case d3d9_window_type::RESIZABLE_WINDOW: return true;
			case d3d9_window_type::VIRTUAL_FULLSCREEN: return true;
			case d3d9_window_type::EXCLUSIVE_FULLSCREEN: return false;
			case d3d9_window_type::CHILD_WINDOW: return true;
			default: ASSERT(false); return false;
		}
	}

}