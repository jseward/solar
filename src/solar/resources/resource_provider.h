#pragma once

#include <string>
#include "resource_provider_type.h"

namespace solar {
	
	class file_system;

	class resource_provider {
	private:
		resource_provider_type _type;
		std::string _root_path;

	public:
		resource_provider();
		resource_provider& build_as_file_system(file_system& file_system, const std::string& root_path);

		resource_provider_type get_type() const;
		const std::string& get_root_path() const;
		std::string get_description() const;
	};

}