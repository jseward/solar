#pragma once

namespace solar {

	enum class window_render_state {
		invalid = -1,

		NORMAL,
		DISABLED,
		FOCUSED,
		PRESSED,
		CURSOROVER,

		count
	};

	class window_render_state_details {
	public:
		typedef window_render_state enum_type;
		static const char* get_string(window_render_state e);
	};

}