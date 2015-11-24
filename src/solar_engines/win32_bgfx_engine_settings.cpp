#include "win32_bgfx_engine_settings.h"

#include "solar/utility/unused_parameter.h"
#include "solar/settings/setting_registry.h"

namespace solar {

	win32_bgfx_engine_settings::win32_bgfx_engine_settings() {
	}

	void win32_bgfx_engine_settings::add_to_setting_registry(setting_registry& registry) {
		UNUSED_PARAMETER(registry);
	}

}