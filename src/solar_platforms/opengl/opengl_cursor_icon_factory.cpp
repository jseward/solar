#include "opengl_cursor_icon_factory.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	opengl_cursor_icon_factory::opengl_cursor_icon_factory(opengl_cursor& cursor, resource_system& resource_system)
		: _cursor(cursor)
		, _resource_system(resource_system) {
	}

	opengl_cursor_icon_factory::~opengl_cursor_icon_factory() {
	}

	void opengl_cursor_icon_factory::setup() {
		//todo?
	}

	void opengl_cursor_icon_factory::teardown() {
		//todo?
	}

	cursor_icon* opengl_cursor_icon_factory::get_cursor_icon(const std::string& id, const std::string& id_source_description) {
		//todo?
		UNUSED_PARAMETER(id);
		UNUSED_PARAMETER(id_source_description);
		return &_dummy;
	}

	const resource_factory_caching_context& opengl_cursor_icon_factory::get_caching_context() const {
		return _caching_context;
	}

}