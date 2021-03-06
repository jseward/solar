#include "window_render_state.h"
#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* to_string(window_render_state e) {
		switch (e) {
			RETURN_ENUM_STRING(window_render_state, NORMAL);
			RETURN_ENUM_STRING(window_render_state, DISABLED);
			RETURN_ENUM_STRING(window_render_state, FOCUSED);
			RETURN_ENUM_STRING(window_render_state, PRESSED);
			RETURN_ENUM_STRING(window_render_state, HOVERED);
			END_ENUM_STRINGS(e);
		}
	}

}