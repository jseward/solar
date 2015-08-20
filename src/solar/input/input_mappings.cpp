#include "input_mappings.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	input_mappings::input_mappings() {
	}

	input_mappings::input_mappings(const input_mapping& mapping) {
		_mappings.push_back(mapping);
	}

	bool input_mappings::has_mapping(const input_mapping& in_mapping) const {
		auto iter = find_if(_mappings, [&in_mapping](const input_mapping& mapping) {
			return mapping == in_mapping;
		});
		return (iter != _mappings.end());
	}

	bool input_mappings::has_mapping_ignore_modifiers(const input_mapping& in_mapping) const {
		auto iter = find_if(_mappings, [&in_mapping](const input_mapping& mapping) {
			return input_mapping::is_equal_ignore_modifiers(in_mapping, mapping);
		});
		return (iter != _mappings.end());
	}

}