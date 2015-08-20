#pragma once

#include "solar/resources/resource_id.h"
#include "mesh.h"
#include "mesh_factory.h"
#include "mesh_texture_params.h"

namespace solar {

	class mesh_id : public resource_id<mesh, mesh_factory> {
	private:
		mesh_texture_params _texture_params;

	public:
		mesh_id(const mesh_texture_params& texture_params);
		virtual int get_current_resource_caching_context(const mesh_factory& factory) const override;
		virtual mesh* get_uncached_resource(mesh_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}