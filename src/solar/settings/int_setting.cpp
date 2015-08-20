#include "int_setting.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {
		
	int_setting::int_setting(const char* group_name, const char* name, int default_value)
		: setting_template<int>(setting_value_type::INT, group_name, name, default_value) {
	}

	checksum int_setting::get_default_value_checksum() const {
		return checksum().add_int(get_default_value());
	}

	void int_setting::read_from_archive(archive_reader& reader, const char* name) {
		int read_value = 0;
		read_int(reader, name, read_value);
		set_current_value(read_value);
	}

	void int_setting::write_to_archive(archive_writer& writer, const char* name) const {
		write_int(writer, name, get_current_value());
	}

}