#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/meshes/mesh_factory.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_context;
	class d3d9_mesh;
	class resource_system;
	class resource_mapped_memory;

	class d3d9_mesh_factory 
		: public mesh_factory
		, public d3d9_device_event_handler {

	private:
		d3d9_context& _context;
		resource_system& _resource_system;
		bool _is_setup;
		int _mesh_caching_context;
		std::unordered_map<std::string, std::unique_ptr<d3d9_mesh>> _meshs;

	public:
		d3d9_mesh_factory(d3d9_context& context, resource_system& resource_system);
		virtual ~d3d9_mesh_factory();

		void setup();
		void teardown();

		virtual mesh* get_mesh(const mesh_texture_params& texture_params, const std::string& id, const std::string& id_source_description) override;
		virtual int get_mesh_caching_context() const override;

		d3d9_context& get_d3d9_context();
		resource_system& get_resource_system();

	private:
		void remove_all_meshs();

		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}
