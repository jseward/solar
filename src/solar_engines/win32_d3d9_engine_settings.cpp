#include "win32_d3d9_engine_settings.h"

#include "solar/settings/setting_registry.h"

namespace solar {

	win32_d3d9_engine_settings::win32_d3d9_engine_settings() {
		#ifdef SOLAR__FILE_WATCHER_DISABLED_BY_DEFAULT
		bool file_watcher_enabled_by_default = false;
		#else
		bool file_watcher_enabled_by_default = true;
		#endif

		_file_watcher_enabled = std::make_shared<bool_setting>("engine", "file_watcher_enabled", file_watcher_enabled_by_default);
		_window_type = std::make_shared<enum_setting<d3d9_window_type_details>>("engine", "window_type", d3d9_window_type::RESIZABLE_WINDOW);
	}

	void win32_d3d9_engine_settings::add_to_setting_registry(setting_registry& registry) {
		registry.add_setting(_file_watcher_enabled);
		registry.add_setting(_window_type);
	}

}