#pragma once

#include "solar/rendering/cursors/cursor_icon_factory.h"
#include "bgfx_cursor_icon.h"

namespace solar {

	class bgfx_cursor;
	class resource_system;

	class bgfx_cursor_icon_factory : public cursor_icon_factory {
	private:
		bgfx_cursor& _cursor;
		resource_system& _resource_system;

		bgfx_cursor_icon _dummy;
		resource_factory_caching_context _caching_context;

	public:
		bgfx_cursor_icon_factory(bgfx_cursor& cursor, resource_system& resource_system);
		virtual ~bgfx_cursor_icon_factory();

		void setup();
		void teardown();

		virtual cursor_icon* get_cursor_icon(const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_caching_context() const override;
	};


}