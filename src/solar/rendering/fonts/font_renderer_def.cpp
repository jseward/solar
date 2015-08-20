#include "font_renderer_def.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	font_renderer_def::font_renderer_def() {
	}

	void font_renderer_def::read_from_archive(archive_reader& reader) {
		read_object(reader, "normal_shader_id", _normal_shader_id);
		read_object(reader, "dropshadow_shader_id", _dropshadow_shader_id);
	}

	void font_renderer_def::write_to_archive(archive_writer& writer) const {
		write_object(writer, "normal_shader_id", _normal_shader_id);
		write_object(writer, "dropshadow_shader_id", _dropshadow_shader_id);
	}

}
