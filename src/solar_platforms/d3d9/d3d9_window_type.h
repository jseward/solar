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

	struct d3d9_window_type_details {
		typedef d3d9_window_type enum_type;
		static const char* get_string(d3d9_window_type e);
		static bool is_device_windowed(d3d9_window_type e);
	};

}