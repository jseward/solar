#include "trace.h"

#include <iostream>
#include "debugging.h"

namespace solar {

	namespace trace_impl {
		static trace_handler s_trace_handler = nullptr;
	}

	void set_trace_handler(trace_handler handler) {
		trace_impl::s_trace_handler = handler;
	}

	void handle_trace(const char* file_name, int line_number, bool add_newline, const char* message) {
		if (trace_impl::s_trace_handler != nullptr) {
			trace_impl::s_trace_handler(file_name, line_number, add_newline, message);
		}
		else {
			print_to_debug_output(message);
			print_to_debug_output("\n");
		}
	}

}