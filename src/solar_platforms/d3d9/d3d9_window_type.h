#pragma once

namespace solar {
	
	enum class d3d9_window_type {
		invalid = -1,
		
		RESIZABLE_WINDOW,
		VIRTUAL_FULLSCREEN,
		EXCLUSIVE_FULLSCREEN,
		CHILD_WINDOW,
		
		count
	};

	extern const char* to_string(d3d9_window_type e);
	extern bool d3d9_window_type_is_device_windowed(d3d9_window_type e);

}