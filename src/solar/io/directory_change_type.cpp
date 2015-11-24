#include "directory_change_type.h"

#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* to_string(directory_change_type type) {
		switch (type) {
			RETURN_ENUM_STRING(directory_change_type, ITEM_ADDED);
			RETURN_ENUM_STRING(directory_change_type, ITEM_REMOVED);
			RETURN_ENUM_STRING(directory_change_type, ITEM_MODIFIED);
			RETURN_ENUM_STRING(directory_change_type, ITEM_RENAMED);
			END_ENUM_STRINGS(type);
		}
	}

	std::ostream& operator<<(std::ostream& os, directory_change_type type) {
		return os << to_string(type);
	}

}