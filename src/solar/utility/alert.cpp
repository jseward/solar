#include "alert.h"
#include "solar/strings/string_build.h"

namespace solar {

	namespace alert_impl {
		static alert_handler s_alert_handler = nullptr;
	}

	void set_alert_handler(alert_handler handler) {
		alert_impl::s_alert_handler = handler;
	}

	bool handle_alert(const char* file_name, int line_number, const char* message) {
		if (alert_impl::s_alert_handler != nullptr) {
			return alert_impl::s_alert_handler(file_name, line_number, message);
		}
		else {
			print_to_debug_output("-------\n");
			print_to_debug_output(build_string("ALERT @ {0}:{1}\n", file_name, line_number).c_str());
			print_to_debug_output(message);
			print_to_debug_output("\n");
			print_to_debug_output("-------\n");
			return true;
		}
	}

}