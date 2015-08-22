#pragma once

#include "solar/resources/resource_id.h"
#include "font.h"
#include "font_factory.h"

namespace solar {

	class font_id : public resource_id<font, font_factory> {
	public:
		font_id();
		virtual const resource_factory_caching_context& get_factory_caching_context(const font_factory& factory) const override;
		virtual font* get_uncached_resource(font_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}