#include "window_font_def.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_enum_helpers.h"

namespace solar {

	window_font_def::window_font_def() 
		: _font_id()
		, _font_size(0.f)
		, _alignment(font_alignment::invalid)
		, _is_multiline(false) 
		, _is_clipping_enabled(false)
		, _color()
		, _is_dropshadow_enabled(false)
		, _dropshadow_def() {
	}

	void window_font_def::read_from_archive(archive_reader& reader) {
		read_object(reader, "font_id", _font_id);
		read_float(reader, "font_size", _font_size);
		read_enum_as_string<font_alignment>(reader, "alignment", _alignment);
		read_bool(reader, "is_multiline", _is_multiline);
		read_bool(reader, "is_clipping_enabled", _is_clipping_enabled);
		read_color(reader, "color", _color);
		read_bool(reader, "is_dropshadow_enabled", _is_dropshadow_enabled);
		if (_is_dropshadow_enabled) {
			read_object(reader, "dropshadow_def", _dropshadow_def);
		}
	}

	void window_font_def::write_to_archive(archive_writer& writer) const {
		write_object(writer, "font_id", _font_id);
		write_float(writer, "font_size", _font_size);
		write_enum_as_string<font_alignment>(writer, "alignment", _alignment);
		write_bool(writer, "is_multiline", _is_multiline);
		write_bool(writer, "is_clipping_enabled", _is_clipping_enabled);
		write_color(writer, "color", _color);
		write_bool(writer, "is_dropshadow_enabled", _is_dropshadow_enabled);
		if (_is_dropshadow_enabled) {
			write_object(writer, "dropshadow_def", _dropshadow_def);
		}
	}

}