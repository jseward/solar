#pragma once

#include <vector>

namespace solar {

	class find_file_names_options {
	public:
		bool _is_recursive;
		std::vector<std::string> _filtered_extensions;

	public:
		find_file_names_options() 
			: _is_recursive(true) {
		}

		find_file_names_options& set_is_recursive(bool is_recursive) {
			_is_recursive = is_recursive;
			return *this;
		}

		find_file_names_options& add_filtered_extension(const std::string& ext) {
			_filtered_extensions.push_back(ext);
			return *this;
		}

		template<typename Con>
		find_file_names_options& add_filtered_extensions(const Con& exts) {
			for (const auto& ext : exts) {
				_filtered_extensions.push_back(ext);
			}
			return *this;
		}

	};


}