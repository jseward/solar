#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/textures/texture_factory.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_context;
	class d3d9_texture;
	class d3d9_texture_pool;
	class resource_system;
	class resource_mapped_memory;

	class d3d9_texture_factory 
		: public texture_factory
		, public d3d9_device_event_handler {

	private:
		d3d9_context& _context;
		resource_system& _resource_system;
		bool _is_setup;
		std::unordered_map<resource_address, std::unique_ptr<d3d9_texture>, resource_address_hasher> _textures;
		std::unique_ptr<resource_mapped_memory> _resource_mapped_memory;

	public:
		d3d9_texture_factory(d3d9_context& context, resource_system& resource_system);
		virtual ~d3d9_texture_factory();

		void setup();
		void teardown();

		virtual texture* create_texture(const resource_address& address, const texture_create_params& params) override;
		virtual void release_texture(texture* texture) override;

		d3d9_context& get_d3d9_context();
		resource_system& get_resource_system();
		resource_mapped_memory& get_resource_mapped_memory();

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}
