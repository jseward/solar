#include "font_id.h"

namespace solar {

	font_factory* font_id::_s_factory = nullptr;

	font_id::font_id() {
	}

	const resource_factory_caching_context& font_id::get_factory_caching_context(const font_factory& factory) const {
		return factory.get_caching_context();
	};

	font* font_id::get_uncached_resource(font_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_font(id, id_source_description);
	}

}