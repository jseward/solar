#include "bool_setting.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {
		
	bool_setting::bool_setting(const char* group_name, const char* name) 
		: setting_template<bool>(setting_value_type::BOOL, group_name, name, false) {
	}

	bool_setting::bool_setting(const char* group_name, const char* name, bool default_value)
		: setting_template<bool>(setting_value_type::BOOL, group_name, name, default_value) {
	}

	checksum bool_setting::get_default_value_checksum() const {
		return checksum().add_bool(get_default_value());
	}

	void bool_setting::read_from_archive(archive_reader& reader, const char* name) {
		bool read_value = false;
		read_bool(reader, name, read_value);
		set_current_value(read_value);
	}

	void bool_setting::write_to_archive(archive_writer& writer, const char* name) const {
		write_bool(writer, name, get_current_value());
	}

}