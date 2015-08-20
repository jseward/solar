#pragma once

#include <vector>
#include "solar/archiving/archivable.h"
#include "mesh_material.h"
#include "mesh_triangle.h"
#include "mesh_vertex.h"

namespace solar {

	class mesh_def : archivable {
	public:
		std::vector<mesh_material> _materials;
		std::vector<mesh_triangle> _triangles;
		std::vector<mesh_vertex> _vertices;

	public:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}