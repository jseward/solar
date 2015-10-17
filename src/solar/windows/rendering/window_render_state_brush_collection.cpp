#include "window_render_state_brush_collection.h"

#include "solar/archiving/archiving_enum_helpers.h"

namespace solar {

	window_render_state_brush_collection::window_render_state_brush_collection() {
	}

	const brush_id& window_render_state_brush_collection::get(window_render_state state) const {
		return _brush_ids[state];
	}

	void window_render_state_brush_collection::read_from_archive(archive_reader& reader, const char* name) {
		read_enum_array<brush_id_array, window_render_state>(reader, name, _brush_ids);
	}

	void window_render_state_brush_collection::write_to_archive(archive_writer& writer, const char* name) const {
		write_enum_array<brush_id_array, window_render_state>(writer, name, _brush_ids);
	}

}
