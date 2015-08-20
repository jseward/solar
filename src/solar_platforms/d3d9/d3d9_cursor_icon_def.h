#pragma once

#include "solar/archiving/archivable.h"
#include "solar/math/point.h"

namespace solar {

	class d3d9_cursor_icon_def : public archivable {
	public:
		std::string _texture_name;
		point _hotspot_offset;

	public:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}