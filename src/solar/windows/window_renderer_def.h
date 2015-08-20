#pragma once

#include "solar/archiving/archivable.h"
#include "solar/rendering/shaders/shader_id.h"
#include "solar/containers/enum_array.h"
#include "window_render_state.h"

namespace solar {

	class window_renderer_def : public archivable {
	public:
		using window_render_state_shader_id_array = enum_array<shader_id, window_render_state>;

	public:
		window_render_state_shader_id_array _brush_shader_ids;

	public:
		window_renderer_def();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}