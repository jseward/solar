#pragma once

namespace solar {
	
	enum class opengl_window_type {
		invalid = -1,
		
		RESIZABLE_WINDOW,
		VIRTUAL_FULLSCREEN,
		
		count
	};

	extern const char* to_string(opengl_window_type e);

}