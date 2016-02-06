#include "resource_id_manifest.h"

#include "resource_system.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/json/json_document.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	resource_id_manifest::resource_id_manifest() {
	}

	void resource_id_manifest::load(resource_system& resource_system, const char* resource_type_name, const char* folder, const char* extension, const char* manifest_id, const char* source_description) {
		ASSERT(_address.empty());
		ASSERT(_ids.empty());

		_address = resource_system.resolve_address_to_file(resource_type_name, folder, extension, manifest_id, source_description);
		if (!_address.empty()) {
			auto stream = resource_system.open_stream_to_read(_address);
			if (stream != nullptr) {
				auto doc = json_document(*stream);
				auto arr = convert_json_document_to_json_array(doc);

				_ids.reserve(arr.size());
				for (unsigned int i = 0; i < arr.size(); ++i) {
					std::string id = arr.get_string(i);
					if (any(_ids, id)) {
						ALERT("duplicate resource_id found.\n\nresource_id : {0}\naddress : {1}", id, _address.to_string());
					}
					else {
						_ids.push_back(id);
					}
				}

				resource_system.close_stream(stream);
			}
		}
	}
	
	const resource_address& resource_id_manifest::get_address() const {
		return _address;
	}

	const std::vector<std::string>& resource_id_manifest::get_ids() const {
		return _ids;
	}

}