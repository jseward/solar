#pragma once

#include <vector>
#include <string>
#include "solar/resources/resource_address.h"

namespace solar {

	class resource_system;

	class resource_id_manifest {
	private:
		resource_address _address;
		std::vector<std::string> _ids;

	public:
		resource_id_manifest();
		void load(resource_system& resource_system, const char* resource_type_name, const char* folder, const char* extension, const char* manifest_id, const char* source_description);
		const std::vector<std::string>& get_ids() const;
		const resource_address& get_address() const;
	};

}