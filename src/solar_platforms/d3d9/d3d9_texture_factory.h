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
		resource_factory_caching_context _caching_context;
		std::unordered_map<std::string, std::unique_ptr<d3d9_texture>> _textures;
		std::unordered_map<std::string, std::unique_ptr<d3d9_texture_pool>> _texture_pools;
		std::unique_ptr<d3d9_texture_pool> _null_texture_pool;
		std::unique_ptr<resource_mapped_memory> _resource_mapped_memory;

	public:
		d3d9_texture_factory(d3d9_context& context, resource_system& resource_system);
		virtual ~d3d9_texture_factory();

		void setup();
		void teardown();

		virtual void add_texture_pool(const char* texture_pool_name, unsigned int max_size_in_bytes, bool should_create_mip_maps) override;
		virtual texture* get_texture(const char* texture_pool_name, const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_caching_context() const override;

		d3d9_context& get_d3d9_context();
		resource_system& get_resource_system();
		resource_mapped_memory& get_resource_mapped_memory();

	private:
		void remove_all_textures();

		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}
