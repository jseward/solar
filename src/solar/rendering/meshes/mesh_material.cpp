#include "mesh_material.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	mesh_material::mesh_material() {
	}

	void mesh_material::read_from_archive(archive_reader& reader) {
		read_string(reader, "diffuse_map", _diffuse_map);
		read_string(reader, "normal_map", _normal_map);
	}

	void mesh_material::write_to_archive(archive_writer& writer) const {
		write_string(writer, "diffuse_map", _diffuse_map);
		write_string(writer, "normal_map", _normal_map);
	}

}