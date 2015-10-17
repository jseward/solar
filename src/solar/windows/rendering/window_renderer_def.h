#pragma once

#include "solar/archiving/archivable.h"
#include "solar/rendering/shaders/shader_id.h"
#include "solar/containers/enum_array.h"

namespace solar {

	class window_renderer_def : public archivable {
	public:
		shader_id _brush_shader;

	public:
		window_renderer_def();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}