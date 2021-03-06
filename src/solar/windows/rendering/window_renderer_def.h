#pragma once

#include "solar/archiving/archivable.h"
#include "solar/rendering/shaders/shader_program_id.h"
#include "solar/rendering/render_states/render_state_block_ptr.h"
#include "solar/containers/enum_array.h"

namespace solar {

	class render_device;

	class window_renderer_def : public archivable {
	public:
		//nothing here anymore, but if custom shaders or other stuff is required this is where they would go

	public:
		window_renderer_def();
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}