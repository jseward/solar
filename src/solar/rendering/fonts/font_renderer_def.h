#pragma once

#include "solar/archiving/archivable.h"
#include "solar/rendering/shaders/shader_program_id.h"

namespace solar {

	class font_renderer_def : public archivable {
	public:
		shader_program_id _normal_shader_program;
		shader_program_id _dropshadow_shader_program;

	public:
		font_renderer_def();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}