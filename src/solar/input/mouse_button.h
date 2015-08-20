#pragma once

namespace solar {

	enum class mouse_button {
		invalid = -1,

		LEFT,
		RIGHT,
		MIDDLE,
		EXTRA_0,
		EXTRA_1,

		count
	};

	struct mouse_button_details {
		static const char* get_string(mouse_button e);
	};
}