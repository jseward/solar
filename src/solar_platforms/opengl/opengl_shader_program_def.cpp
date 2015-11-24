#include "opengl_shader_program_def.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	void opengl_shader_program_def::read_from_archive(archive_reader& reader) {
		read_string(reader, "vertex_shader_name", _vertex_shader_name);
		read_string(reader, "fragment_shader_name", _fragment_shader_name);
	}
		
	void opengl_shader_program_def::write_to_archive(archive_writer& writer) const {
		write_string(writer, "vertex_shader_name", _vertex_shader_name);
		write_string(writer, "fragment_shader_name", _fragment_shader_name);
	}
	
}