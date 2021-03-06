#include "resource_address.h"

#include "solar/utility/assert.h"
#include "solar/io/file_path_helpers.h"

namespace solar {

	resource_address::resource_address()
		: _is_file(false)
		, _provider_type(resource_provider_type::invalid) {
	}

	bool resource_address::operator==(const resource_address& rhs) const {
		return
			_is_file == rhs._is_file &&
			_provider_type == rhs._provider_type &&
			_path == rhs._path;
	}

	bool resource_address::operator!=(const resource_address& rhs) const {
		return !(*this == rhs);
	}

	resource_address make_resource_address_with_file_system_provider(bool is_file, const std::string& path) {
		ASSERT(!path.empty());
		resource_address address;
		address._is_file = is_file;
		address._provider_type = resource_provider_type::FILE_SYSTEM;
		address._path = path;
		return address;
	}

	bool resource_address::empty() const {
		return (_provider_type == resource_provider_type::invalid);
	}

	bool resource_address::is_file() const {
		return _is_file;
	}

	bool resource_address::is_directory() const {
		return !_is_file;
	}

	resource_provider_type resource_address::get_provider_type() const {
		return _provider_type;
	}

	const std::string& resource_address::get_path() const {
		return _path;
	}

	std::string resource_address::get_file_extension() const {
		ASSERT(_is_file);
		return solar::get_file_extension(_path);
	}

	std::string resource_address::to_string() const {
		//perhaps fancier string when different provider_types are added...
		return _path;
	}

	std::ostream& operator<<(std::ostream& os, const resource_address& that) {
		return os << that.to_string();
	}

	std::size_t resource_address::get_hash() const {
		return
			(std::hash<bool>()(_is_file)) ^
			(std::hash<resource_provider_type>()(_provider_type) << 1) ^
			(std::hash<std::string>()(_path) << 2);
	}

}
