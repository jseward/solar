#pragma once

#include <string>

namespace solar {

	template<typename CollectionT>
	std::string join_strings(const char* seperator, const CollectionT& collection) {
		std::stringstream ss;
		bool is_first = true;
		for (auto iter : collection) {
			if (!is_first) {
				ss << seperator;
			}
			is_first = false;
			ss << iter;
		}
		return ss.str();
	}

}