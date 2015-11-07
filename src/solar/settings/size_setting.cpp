#include "size_setting.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {
		
	size_setting::size_setting(const char* group_name, const char* name, const size& default_value)
		: setting_template<size>(setting_value_type::SIZE, group_name, name, default_value) {
	}

	checksum size_setting::get_default_value_checksum() const {
		return checksum().add_size(get_default_value());
	}

	void size_setting::read_from_archive(archive_reader& reader, const char* name) {
		size read_value;
		read_size(reader, name, read_value);
		set_current_value(read_value);
	}

	void size_setting::write_to_archive(archive_writer& writer, const char* name) const {
		write_size(writer, name, get_current_value());
	}

}