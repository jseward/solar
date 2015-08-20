#include "debugging.h"

namespace solar {

	void print_to_debug_output(const std::string& string) {
		print_to_debug_output(string.c_str());
	}

}
