#include "window_renderer_def.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_renderer_def::window_renderer_def() {
	}

	void window_renderer_def::read_from_archive(archive_reader& reader) {
		read_object(reader, "brush_shader_program", _brush_shader_program);
	}

	void window_renderer_def::write_to_archive(archive_writer& writer) const {
		write_object(writer, "brush_shader_program", _brush_shader_program);
	}

}
