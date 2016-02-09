#pragma once

#include "solar/resources/resource_factory_caching_context.h"

namespace solar {

	class mesh;

	class mesh_factory {
	public:
		virtual mesh* get_mesh(const std::string& id, const std::string& id_source_description) = 0;
		virtual const resource_factory_caching_context& get_caching_context() const = 0;
	};

}