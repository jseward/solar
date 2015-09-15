#include "float_setting.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {
		
	float_setting::float_setting(const char* group_name, const char* name, float default_value)
		: setting_template<float>(setting_value_type::INT, group_name, name, default_value) {
	}

	checksum float_setting::get_default_value_checksum() const {
		return checksum().add_float(get_default_value());
	}

	void float_setting::read_from_archive(archive_reader& reader, const char* name) {
		float read_value = 0;
		read_float(reader, name, read_value);
		set_current_value(read_value);
	}

	void float_setting::write_to_archive(archive_writer& writer, const char* name) const {
		write_float(writer, name, get_current_value());
	}

}