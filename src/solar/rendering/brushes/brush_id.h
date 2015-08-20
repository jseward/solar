#pragma once

#include "solar/resources/resource_id.h"
#include "brush.h"
#include "brush_factory.h"

namespace solar {

	class brush_id : public resource_id<brush, brush_factory> {
	public:
		brush_id();
		virtual int get_current_resource_caching_context(const brush_factory& factory) const override;
		virtual brush* get_uncached_resource(brush_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}