#include "label_style.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	window_style_factory* label_style_id::_s_factory = nullptr;

	const char* label_style::TYPE_NAME = "label";

	std::unique_ptr<window_style> label_style::clone() const {
		return std::make_unique<label_style>();
	}

	void label_style::read_from_archive(archive_reader& reader) {
		UNUSED_PARAMETER(reader);
	}

	void label_style::write_to_archive(archive_writer& writer) const {
		UNUSED_PARAMETER(writer);
	}

}
