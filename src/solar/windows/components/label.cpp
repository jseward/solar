#include "label.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	label::label(const char* id)
		: window_component(id)
		, _style(*this) {

		set_has_layout(true);
	}

	label::~label() {
	}

	void label::render(const window_render_params& params) {
		UNUSED_PARAMETER(params);
		//todo
	}

	void label::read_from_archive(archive_reader& reader) {
		window::read_from_archive(reader);
		read_object(reader, "style", _style);
	}

	void label::write_to_archive(archive_writer& writer) const {
		window::write_to_archive(writer);
		write_object(writer, "style", _style);
	}

}