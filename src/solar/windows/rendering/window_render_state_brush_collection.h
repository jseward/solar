#pragma once

#include "window_render_state.h"
#include "solar/archiving/archivable.h"
#include "solar/containers/enum_array.h"
#include "solar/rendering/brushes/brush_id.h"

namespace solar {

	class window_render_state_brush_collection : public single_value_archivable {
	private:
		using brush_id_array = enum_array<brush_id, window_render_state>;

	private:
		brush_id_array _brush_ids;

	public:
		window_render_state_brush_collection();

		const brush_id& get(window_render_state state) const;

		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;
	};

}