#include "win32_skip_error_registry.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	win32_skip_error_registry::location::location() 
		: _file_name("")
		, _line_number(0) {
	}

	win32_skip_error_registry::location::location(const char* file_name, int line_number) 
		: _file_name(file_name)
		, _line_number(line_number) {
	}

	bool win32_skip_error_registry::location::operator==(const location& rhs) const {
		return
			(_file_name == rhs._file_name) &&
			(_line_number == rhs._line_number);
	}

	win32_skip_error_registry::win32_skip_error_registry() {
	}

	bool win32_skip_error_registry::contains(const char* file_name, int line_number) const {
		return any(_locations, location(file_name, line_number));
	}

	void win32_skip_error_registry::add(const char* file_name, int line_number) {
		_locations.push_back(location(file_name, line_number));
	}

}