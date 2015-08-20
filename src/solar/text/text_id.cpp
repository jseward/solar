#include "text_id.h"

namespace solar {

	text_factory* text_id::_s_factory = nullptr;

	int text_id::get_current_resource_caching_context(const text_factory& factory) const {
		return factory.get_text_caching_context();
	}

	text* text_id::get_uncached_resource(text_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_text(id, id_source_description);
	}

}