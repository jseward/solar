#pragma once

#include <memory>
#include "solar/settings/size_setting.h"
#include "solar/settings/enum_setting.h"
#include "solar_platforms/d3d9/d3d9_window_type.h"

namespace solar {

	class setting_registry;

	class win32_d3d9_engine_settings {
	public:
		std::shared_ptr<enum_setting<d3d9_window_type>> _window_type;
		std::shared_ptr<size_setting> _window_size;

	public:
		win32_d3d9_engine_settings();

		void add_to_setting_registry(setting_registry& registry);
	};

}