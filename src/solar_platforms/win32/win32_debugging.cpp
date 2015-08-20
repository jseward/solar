#include "solar/utility/debugging.h"

#include "win32_headers.h"

namespace solar {

	void print_to_debug_output(const char* string) {
		::OutputDebugStringA(string);
	}

}