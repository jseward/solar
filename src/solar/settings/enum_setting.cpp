#include "int_setting.h"

namespace solar {
		
	int_setting::int_setting(const char* group_name, const char* name)
		: setting_template<int>(setting_value_type::INT, group_name, name, 0) {
	}

}