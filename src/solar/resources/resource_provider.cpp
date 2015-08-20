#include "resource_provider.h"
#include "solar/strings/string_build.h"
#include "solar/io/file_system.h"

namespace solar {
	
	resource_provider::resource_provider()
		: _type(resource_provider_type::invalid) {
	}

	resource_provider& resource_provider::build_as_file_system(file_system& file_system, const std::string& root_path) {
		_type = resource_provider_type::FILE_SYSTEM;
		_root_path = file_system.resolve_file_path(root_path);
		return *this;
	}

	resource_provider_type resource_provider::get_type() const {
		return _type;
	}

	const std::string& resource_provider::get_root_path() const {
		return _root_path;
	}

	std::string resource_provider::get_description() const {
		if (_type == resource_provider_type::FILE_SYSTEM) {
			return build_string("[FILE_SYSTEM] {}", _root_path);
		}
		return "???";
	}

}