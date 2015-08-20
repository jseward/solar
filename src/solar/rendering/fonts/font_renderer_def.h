#pragma once

#include "solar/archiving/archivable.h"
#include "solar/rendering/shaders/shader_id.h"

namespace solar {

	class font_renderer_def : public archivable {
	public:
		shader_id _normal_shader_id;
		shader_id _dropshadow_shader_id;

	public:
		font_renderer_def();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}