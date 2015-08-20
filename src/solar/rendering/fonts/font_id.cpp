#include "font_id.h"

namespace solar {

	font_factory* font_id::_s_factory = nullptr;

	font_id::font_id() {
	}

	int font_id::get_current_resource_caching_context(const font_factory& factory) const {
		return factory.get_font_caching_context();
	};

	font* font_id::get_uncached_resource(font_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_font(id, id_source_description);
	}

}