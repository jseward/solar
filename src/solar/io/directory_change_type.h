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

	std::ostream& operator<<(std::ostream& os, directory_change_type e);

	class directory_change_type_details {
	public:
		static const char* get_string(directory_change_type e);
	};

}