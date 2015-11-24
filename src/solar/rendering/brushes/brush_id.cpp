#include "brush_id.h"

namespace solar {

	brush_factory* brush_id::_s_factory = nullptr;

	brush_id::brush_id() {
	}

	const resource_factory_caching_context& brush_id::get_factory_caching_context(const brush_factory& factory) const {
		return factory.get_caching_context();
	}

	brush* brush_id::get_uncached_resource(brush_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_brush(id, id_source_description);
	}

}