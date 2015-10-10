#include "directory_change_type.h"

#include "solar/utility/enum_helpers.h"

namespace solar {

	std::ostream& operator<<(std::ostream& os, directory_change_type e) {
		return os << directory_change_type_details::get_string(e);
	}

	const char* directory_change_type_details::get_string(directory_change_type e) {
		switch (e) {
			RETURN_ENUM_STRING(directory_change_type, ITEM_ADDED);
			RETURN_ENUM_STRING(directory_change_type, ITEM_REMOVED);
			RETURN_ENUM_STRING(directory_change_type, ITEM_MODIFIED);
			RETURN_ENUM_STRING(directory_change_type, ITEM_RENAMED);
			END_ENUM_STRINGS(e);
		}
	}

}