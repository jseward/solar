#include "d3d9_cursor_icon_def.h"

#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	void d3d9_cursor_icon_def::read_from_archive(archive_reader& reader) {
		read_string(reader, "texture_name", _texture_name);
		read_point(reader, "hotspot_offset", _hotspot_offset);
	}

	void d3d9_cursor_icon_def::write_to_archive(archive_writer& writer) const {
		write_string(writer, "texture_name", _texture_name);
		write_point(writer, "hotspot_offset", _hotspot_offset);
	}

}