#pragma once

#include "solar/resources/resource_id.h"
#include "cursor_icon.h"
#include "cursor_icon_factory.h"

namespace solar {

	class cursor_icon_id : public resource_id<cursor_icon, cursor_icon_factory> {
	public:
		virtual const resource_factory_caching_context& get_factory_caching_context(const cursor_icon_factory& factory) const override;
		virtual cursor_icon* get_uncached_resource(cursor_icon_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}