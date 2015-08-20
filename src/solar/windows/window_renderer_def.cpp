#include "window_renderer_def.h"
#include "solar/archiving/archiving_enum_helpers.h"

namespace solar {

	window_renderer_def::window_renderer_def() {
	}

	void window_renderer_def::read_from_archive(archive_reader& reader) {
		read_enum_array<window_render_state_shader_id_array, window_render_state_details>(reader, "brush_shader_ids", _brush_shader_ids);
	}

	void window_renderer_def::write_to_archive(archive_writer& writer) const {
		write_enum_array<window_render_state_shader_id_array, window_render_state_details>(writer, "brush_shader_ids", _brush_shader_ids);
	}

}
