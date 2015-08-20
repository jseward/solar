#include "mesh_vertex.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	mesh_vertex::mesh_vertex()
		: _binormal_sign(1.f) {
	}

	void mesh_vertex::read_from_archive(archive_reader& reader) {
		read_vec3(reader, "position", _position);
		read_vec3(reader, "normal", _normal);
		read_vec3(reader, "tangent", _tangent);
		read_float(reader, "binormal_sign", _binormal_sign);
		read_object(reader, "uv", _uv);
		read_object_fixed_vector(reader, "bone_weights", _bone_weights);
	}

	void mesh_vertex::write_to_archive(archive_writer& writer) const {
		write_vec3(writer, "position", _position);
		write_vec3(writer, "normal", _normal);
		write_vec3(writer, "tangent", _tangent);
		write_float(writer, "binormal_sign", _binormal_sign);
		write_object(writer, "uv", _uv);
		write_object_fixed_vector(writer, "bone_weights", _bone_weights);
	}

}