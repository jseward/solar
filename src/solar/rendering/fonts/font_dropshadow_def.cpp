#include "font_dropshadow_def.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	font_dropshadow_def::font_dropshadow_def()
		: _offset()
		, _min_distance(0.f)
		, _max_distance(0.65f)
		, _color(0.f, 0.f, 0.f, 1.f) {
	}

	void font_dropshadow_def::read_from_archive(archive_reader& reader) {
		read_pointf(reader, "offset", _offset);
		read_float(reader, "min_distance", _min_distance);
		read_float(reader, "max_distance", _max_distance);
		read_color(reader, "color", _color);
	}

	void font_dropshadow_def::write_to_archive(archive_writer& writer) const {
		write_pointf(writer, "offset", _offset);
		write_float(writer, "min_distance", _min_distance);
		write_float(writer, "max_distance", _max_distance);
		write_color(writer, "color", _color);
	}

}
