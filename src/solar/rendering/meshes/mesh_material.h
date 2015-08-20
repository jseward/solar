#pragma once

#include <string>
#include "solar/archiving/archivable.h"

namespace solar {

	class mesh_material : archivable {
	public:
		std::string _diffuse_map;
		std::string _normal_map;

	public:
		mesh_material();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}