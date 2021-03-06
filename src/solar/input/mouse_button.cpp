#include "mouse_button.h"

#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* to_string(mouse_button button) {
		switch (button) {
			RETURN_ENUM_STRING(mouse_button, invalid);
			RETURN_ENUM_STRING(mouse_button, LEFT);
			RETURN_ENUM_STRING(mouse_button, RIGHT);
			RETURN_ENUM_STRING(mouse_button, MIDDLE);
			RETURN_ENUM_STRING(mouse_button, EXTRA_0);
			RETURN_ENUM_STRING(mouse_button, EXTRA_1);
			END_ENUM_STRINGS(button);
		}
	}

}