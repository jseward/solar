#pragma once

#include "solar/resources/resource_id.h"
#include "mesh.h"
#include "mesh_factory.h"

namespace solar {

	class mesh_texture_params {
	public:
		const char* _diffuse_texture_pool_name;
		const char* _normal_texture_pool_name;

	public:
		mesh_texture_params(
			const char* diffuse_texture_pool_name,
			const char* normal_texture_pool_name)
			: _diffuse_texture_pool_name(diffuse_texture_pool_name)
			, _normal_texture_pool_name(normal_texture_pool_name) {
		}

		bool operator==(const mesh_texture_params& rhs) const {
			return
				_diffuse_texture_pool_name == rhs._diffuse_texture_pool_name &&
				_normal_texture_pool_name == rhs._normal_texture_pool_name;
		}

		bool operator!=(const mesh_texture_params& rhs) const {
			return !(*this == rhs);
		}

		friend std::ostream& operator<<(std::ostream& os, const mesh_texture_params& that) {
			return os << "{ diffuse:'" << that._diffuse_texture_pool_name << "' , normal:'" << that._normal_texture_pool_name << "' }";
		}
	};

}