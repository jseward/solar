#pragma once

#include "solar/archiving/archivable.h"

namespace solar {

	class mesh_triangle : archivable {
	public:
		unsigned short _vertex_index_0;
		unsigned short _vertex_index_1;
		unsigned short _vertex_index_2;
		unsigned short _material_index;

	public:
		mesh_triangle();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}