#include "brush_id.h"

namespace solar {

	brush_factory* brush_id::_s_factory = nullptr;

	brush_id::brush_id() {
	}

	int brush_id::get_current_resource_caching_context(const brush_factory& factory) const {
		return factory.get_brush_caching_context();
	}

	brush* brush_id::get_uncached_resource(brush_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_brush(id, id_source_description);
	}

}