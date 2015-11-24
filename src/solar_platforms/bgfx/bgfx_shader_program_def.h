#pragma once

#include <string>
#include "solar/archiving/archivable.h"

namespace solar {

	class bgfx_shader_program_def : public archivable {
	public:
		std::string _vertex_shader_name;
		std::string _fragment_shader_name;

	public:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};
	
}