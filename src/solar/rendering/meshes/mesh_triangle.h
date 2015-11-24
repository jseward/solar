#pragma once

#include <stdint.h>
#include "solar/archiving/archivable.h"

namespace solar {

	class mesh_triangle : archivable {
	public:
		uint16_t _vertex_index_0;
		uint16_t _vertex_index_1;
		uint16_t _vertex_index_2;
		uint16_t _material_index;

	public:
		mesh_triangle();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}