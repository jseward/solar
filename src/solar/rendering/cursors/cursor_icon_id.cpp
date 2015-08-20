#include "cursor_icon_id.h"

namespace solar {

	cursor_icon_factory* cursor_icon_id::_s_factory = nullptr;

	int cursor_icon_id::get_current_resource_caching_context(const cursor_icon_factory& factory) const {
		return factory.get_cursor_icon_caching_context();
	}

	cursor_icon* cursor_icon_id::get_uncached_resource(cursor_icon_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_cursor_icon(id, id_source_description);
	}

}
