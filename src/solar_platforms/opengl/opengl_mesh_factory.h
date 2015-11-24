#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/meshes/mesh_factory.h"

namespace solar {

	class opengl_context;
	class opengl_mesh;
	class resource_system;
	class resource_mapped_memory;

	class opengl_mesh_factory : public mesh_factory {
	private:
		resource_system& _resource_system;
		bool _is_setup;
		resource_factory_caching_context _caching_context;
		std::unordered_map<std::string, std::unique_ptr<opengl_mesh>> _meshs;

	public:
		opengl_mesh_factory(resource_system& resource_system);
		virtual ~opengl_mesh_factory();

		void setup();
		void teardown();

		virtual mesh* get_mesh(const mesh_texture_params& texture_params, const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_caching_context() const override;

		opengl_context& get_opengl_context();
		resource_system& get_resource_system();

	private:
		void remove_all_meshs();
	};

}
