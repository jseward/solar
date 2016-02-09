#pragma once

#include "solar/resources/resource_id.h"
#include "mesh.h"
#include "mesh_factory.h"

namespace solar {

	class mesh_id : public resource_id<mesh, mesh_factory> {
	public:
		mesh_id();
		virtual const resource_factory_caching_context& get_factory_caching_context(const mesh_factory& factory) const override;
		virtual mesh* get_uncached_resource(mesh_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}