#include "bgfx_cursor_icon_factory.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_cursor_icon_factory::bgfx_cursor_icon_factory(bgfx_cursor& cursor, resource_system& resource_system)
		: _cursor(cursor)
		, _resource_system(resource_system) {
	}

	bgfx_cursor_icon_factory::~bgfx_cursor_icon_factory() {
	}

	void bgfx_cursor_icon_factory::setup() {
		//todo?
	}

	void bgfx_cursor_icon_factory::teardown() {
		//todo?
	}

	cursor_icon* bgfx_cursor_icon_factory::get_cursor_icon(const std::string& id, const std::string& id_source_description) {
		//todo?
		UNUSED_PARAMETER(id);
		UNUSED_PARAMETER(id_source_description);
		return &_dummy;
	}

	const resource_factory_caching_context& bgfx_cursor_icon_factory::get_caching_context() const {
		return _caching_context;
	}

}