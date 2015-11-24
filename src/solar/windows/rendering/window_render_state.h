#pragma once

namespace solar {

	enum class window_render_state {
		invalid = -1,

		NORMAL,
		DISABLED,
		FOCUSED,
		PRESSED,
		HOVERED,

		count
	};

	extern const char* to_string(window_render_state e);

}