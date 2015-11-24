#include "window_renderer_def.h"

#include "solar/archiving/archiving_helpers.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	window_renderer_def::window_renderer_def() {
	}

	void window_renderer_def::read_from_archive(archive_reader& reader) {
		UNUSED_PARAMETER(reader);
	}

	void window_renderer_def::write_to_archive(archive_writer& writer) const {
		UNUSED_PARAMETER(writer);
	}

}
