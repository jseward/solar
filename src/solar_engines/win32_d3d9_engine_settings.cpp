#include "win32_d3d9_engine_settings.h"

#include "solar/settings/setting_registry.h"

namespace solar {

	win32_d3d9_engine_settings::win32_d3d9_engine_settings() {
		_window_type = std::make_shared<enum_setting<d3d9_window_type>>("engine", "window_type", d3d9_window_type::RESIZABLE_WINDOW);
		_window_size = std::make_shared<size_setting>("engine", "window_size", size(1024, 768));
	}

	void win32_d3d9_engine_settings::add_to_setting_registry(setting_registry& registry) {
		registry.add_setting(_window_type);
		registry.add_setting(_window_size);
	}

}