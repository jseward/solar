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

	extern const char* to_string(mouse_button button);

}