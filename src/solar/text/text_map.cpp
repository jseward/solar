#include "text_map.h"

#include "solar/archiving/json_archive_reader.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/strings/string_marshal.h"
#include "solar/strings/string_build.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	void text_map::load_from_stream(stream& s) {
		std::string id;
		std::string value_utf8;
		std::wstring value_utf16;

		this->clear();

		json_archive_reader reader(s);
		reader.read_name("text");
		reader.read_array(
			[&](archive_reader&, unsigned int size) {
				this->reserve(size);
				return true;
			},
			[&](archive_reader& reader, unsigned int index) {
				reader.read_object([&](archive_reader& reader) {
					read_string(reader, "id", id);
					read_string(reader, "value", value_utf8);
					utf8_to_utf16(value_utf16, value_utf8.c_str());
					if (this->find(id) != this->end()) {
						reader.raise_error(build_string("duplicate text id found. {{ index:{} }}", index));
					}
					else {
						(*this)[id] = std::make_unique<text>(value_utf16);
					}
				});
			});
	}

	void text_map::save_to_stream(stream& s) {
		UNUSED_PARAMETER(s);
		ASSERT(false);//todo
	}

}