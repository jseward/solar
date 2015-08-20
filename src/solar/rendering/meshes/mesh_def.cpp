#include "mesh_def.h"
#include "solar/utility/assert.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	void mesh_def::read_from_archive(archive_reader& reader) {
		read_object_vector(reader, "materials", _materials);
		read_object_vector(reader, "triangles", _triangles);
		read_object_vector(reader, "vertices", _vertices);
	}

	void mesh_def::write_to_archive(archive_writer& writer) const {
		write_object_vector(writer, "materials", _materials);
		write_object_vector(writer, "triangles", _triangles);
		write_object_vector(writer, "vertices", _vertices);
	}

}
