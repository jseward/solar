#pragma once

namespace solar {

	class mesh;
	class mesh_texture_params;

	class mesh_factory {
	public:
		virtual mesh* get_mesh(const mesh_texture_params& texture_params, const std::string& id, const std::string& id_source_description) = 0;
		virtual int get_mesh_caching_context() const = 0;
	};

}