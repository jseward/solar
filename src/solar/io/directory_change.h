#pragma once

#include <string>
#include <ostream>
#include "directory_change_type.h"

namespace solar {

	class directory_change {
	private:
		directory_change_type _change_type;
		std::string _path;

	public:
		directory_change() 
			: _change_type(directory_change_type::invalid) {
		}

		directory_change(directory_change_type change_type, const std::string& path)
			: _change_type(change_type)
			, _path(path) {
		}

		directory_change_type get_change_type() const {
			return _change_type;
		}

		const std::string& get_path() const {
			return _path;
		}

	public:
		friend std::ostream& operator<<(std::ostream& os, const directory_change& dc) {
			return os << dc.get_path() << " (" << dc._change_type << ")";
		}
	};

}