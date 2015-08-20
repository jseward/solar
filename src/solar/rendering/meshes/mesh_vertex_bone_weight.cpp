#include "mesh_vertex_bone_weight.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	mesh_vertex_bone_weight::mesh_vertex_bone_weight() 
		: _bone_index(0)
		, _weight(0.f) {
	}

	void mesh_vertex_bone_weight::read_from_archive(archive_reader& reader) {
		read_ushort(reader, "bone_index", _bone_index);
		read_float(reader, "weight", _weight);
	}

	void mesh_vertex_bone_weight::write_to_archive(archive_writer& writer) const {
		write_ushort(writer, "bone_index", _bone_index);
		write_float(writer, "weight", _weight);
	}

}