#pragma once

#include <string>
#include <vector>

namespace solar {

	class win32_skip_error_registry {
	private:
		struct location {
			const char* _file_name;
			int _line_number;

		public:
			location();
			location(const char* file_name, int line_number);
			bool operator==(const location& rhs) const;
		};

	private:
		std::vector<location> _locations;

	public:
		win32_skip_error_registry();

		bool contains(const char* file_name, int line_number) const;
		void add(const char* file_name, int line_number);
	};

}
