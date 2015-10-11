#pragma once

#include <ostream>

namespace solar {

	enum class directory_change_type {
		invalid = -1,

		ITEM_ADDED,
		ITEM_REMOVED,
		ITEM_MODIFIED,
		ITEM_RENAMED,
	};

	extern const char* to_string(directory_change_type type);
	extern std::ostream& operator<<(std::ostream& os, directory_change_type type);

}