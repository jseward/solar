#pragma once

#include <vector>
#include "input_mapping.h"

namespace solar {

	class input_mappings {
	private:
		std::vector<input_mapping> _mappings;

	public:
		input_mappings();
		explicit input_mappings(const input_mapping& mapping);

		bool has_mapping(const input_mapping& in_mapping) const;
		bool has_mapping_ignore_modifiers(const input_mapping& in_mapping) const;
	};

}