#pragma once

#include <string>
#include "resource_provider_type.h"

namespace solar {

	class resource_address {
	private:
		bool _is_file;
		resource_provider_type _provider_type;
		std::string _path;

	public:
		resource_address();

		bool operator==(const resource_address& rhs) const;
		bool operator!=(const resource_address& rhs) const;

		bool empty() const;
		bool is_file() const;
		bool is_directory() const;
		resource_provider_type get_provider_type() const;
		const std::string& get_path() const;
		std::string get_file_extension() const;
		std::string to_string() const;

	public:
		friend resource_address make_resource_address_with_file_system_provider(bool is_file, const std::string& path);
		friend std::ostream& operator<<(std::ostream& os, const resource_address& that);
	};

}