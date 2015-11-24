#pragma once

#include "solar/rendering/cursors/cursor_icon_factory.h"
#include "opengl_cursor_icon.h"

namespace solar {

	class opengl_cursor;
	class resource_system;

	class opengl_cursor_icon_factory : public cursor_icon_factory {
	private:
		opengl_cursor& _cursor;
		resource_system& _resource_system;

		opengl_cursor_icon _dummy;
		resource_factory_caching_context _caching_context;

	public:
		opengl_cursor_icon_factory(opengl_cursor& cursor, resource_system& resource_system);
		virtual ~opengl_cursor_icon_factory();

		void setup();
		void teardown();

		virtual cursor_icon* get_cursor_icon(const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_caching_context() const override;
	};


}