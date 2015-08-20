#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "solar/rendering/cursors/cursor_icon_factory.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_cursor;
	class d3d9_cursor_icon;
	class d3d9_context;
	class resource_system;
	class resource_mapped_memory;

	class d3d9_cursor_icon_factory 
		: public cursor_icon_factory
		, public d3d9_device_event_handler {

	private:
		d3d9_context& _context;
		d3d9_cursor& _cursor;
		resource_system& _resource_system;
		std::unique_ptr<resource_mapped_memory> _resource_mapped_memory;
		std::unordered_map<std::string, std::unique_ptr<d3d9_cursor_icon>> _icons;
		std::unique_ptr<d3d9_cursor_icon> _empty_icon;
		int _caching_context;

	public:
		d3d9_cursor_icon_factory(d3d9_context& context, d3d9_cursor& cursor, resource_system& resource_system);
		virtual ~d3d9_cursor_icon_factory();

		void setup();
		void teardown();

		virtual cursor_icon* get_cursor_icon(const std::string& id, const std::string& id_source_description) override;
		virtual int get_cursor_icon_caching_context() const override;

		d3d9_cursor& get_cursor();
		resource_system& get_resource_system();
		resource_mapped_memory& get_resource_mapped_memory();

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};


}