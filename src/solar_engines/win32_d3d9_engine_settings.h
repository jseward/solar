#pragma once

#include <memory>
#include "solar/settings/int_setting.h"
#include "solar/settings/enum_setting.h"
#include "solar_platforms/d3d9/d3d9_window_type.h"

namespace solar {

	class setting_registry;

	class win32_d3d9_engine_settings {
	private:
		std::shared_ptr< enum_setting<d3d9_window_type_details> > _window_type;

	public:
		win32_d3d9_engine_settings();

		d3d9_window_type get_window_type() const;

		void add_to_setting_registry(setting_registry& registry);
	};

}