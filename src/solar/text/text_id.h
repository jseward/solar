#pragma once

#include "solar/resources/resource_id.h"
#include "text.h"
#include "text_factory.h"

namespace solar {

	class text_id : public resource_id<text, text_factory> {
	private:
		virtual int get_current_resource_caching_context(const text_factory& factory) const override;
		virtual text* get_uncached_resource(text_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}