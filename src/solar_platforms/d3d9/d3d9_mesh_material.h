#pragma once

#include "solar/rendering/textures/texture_id.h"
#include "solar/rendering/meshes/mesh_material.h"
#include "solar/utility/alert.h"

namespace solar {

	class d3d9_mesh_material {
	public:
		texture_id _diffuse_map;
		texture_id _normal_map;

	public:
		d3d9_mesh_material(const std::string& mesh_file_path, const mesh_material& material)
			: _diffuse_map() 
			, _normal_map() {

			set_id(_diffuse_map, material._diffuse_map, mesh_file_path, "DIFFUSE");
			set_id(_normal_map, material._normal_map, mesh_file_path, "NORMAL");
		}

	private:
		void set_id(texture_id& id, const std::string& value, const std::string& mesh_file_path, const char* texture_type) {
			if (value.empty()) {
				ALERT("d3d9_mesh_material has no {} texture : {}", texture_type, mesh_file_path);
			}
			id.set_id(value.c_str(), mesh_file_path.c_str());
		}
	};

}