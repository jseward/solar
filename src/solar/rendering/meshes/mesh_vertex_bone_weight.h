#pragma once

#include "solar/archiving/archivable.h"

namespace solar {

	class mesh_vertex_bone_weight : archivable {
	public:
		unsigned short _bone_index;
		float _weight;

	public:
		mesh_vertex_bone_weight();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}