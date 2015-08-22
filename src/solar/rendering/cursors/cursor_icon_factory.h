#pragma once

#include <string>
#include "solar/resources/resource_factory_caching_context.h"

namespace solar {

	class cursor_icon;

	class cursor_icon_factory {
	public:
		virtual cursor_icon* get_cursor_icon(const std::string& id, const std::string& id_source_description) = 0;
		virtual const resource_factory_caching_context& get_caching_context() const = 0;
	};

}