#include "mesh_triangle.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	mesh_triangle::mesh_triangle() 
		: _vertex_index_0(0)
		, _vertex_index_1(0)
		, _vertex_index_2(0)
		, _material_index(0) {
	}

	void mesh_triangle::read_from_archive(archive_reader& reader) {
		read_ushorts(reader, "vertices", _vertex_index_0, _vertex_index_1, _vertex_index_2);
		read_ushort(reader, "material", _material_index);
	}

	void mesh_triangle::write_to_archive(archive_writer& writer) const {
		write_ushorts(writer, "vertices", _vertex_index_0, _vertex_index_1, _vertex_index_2);
		write_ushort(writer, "material", _material_index);
	}

}
