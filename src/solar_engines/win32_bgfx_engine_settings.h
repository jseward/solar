#pragma once

#include <memory>
#include "solar/settings/bool_setting.h"
#include "solar/settings/int_setting.h"
#include "solar/settings/enum_setting.h"

namespace solar {

	class setting_registry;

	class win32_bgfx_engine_settings {
	public:
		win32_bgfx_engine_settings();

		void add_to_setting_registry(setting_registry& registry);
	};

}