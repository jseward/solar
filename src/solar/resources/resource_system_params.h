#pragma once

#include <vector>
#include "resource_provider.h"

namespace solar {

	class resource_system_params {
	public:
		bool _is_watching_enabled;
		std::vector<resource_provider> _providers;

	public:
		resource_system_params() 
			: _is_watching_enabled(false) {
		}

		resource_system_params& add_provider(const resource_provider& provider) {
			_providers.push_back(provider);
			return *this;
		}

		resource_system_params& set_is_watching_enabled(bool is_watching_enabled) {
			_is_watching_enabled = is_watching_enabled;
			return *this;
		}
	};

}