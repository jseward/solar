#pragma once

#include "setting_template.h"

namespace solar {

	class bool_setting : public setting_template<bool> {
	public:
		bool_setting(const char* group_name, const char* name);
		virtual checksum get_default_value_checksum() const override;
		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;
	};

}