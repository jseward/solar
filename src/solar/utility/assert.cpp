#include "assert.h"

#include "solar/strings/string_build.h"

namespace solar {

	namespace assert_impl {
		static assert_failed_handler s_assert_failed_handler = nullptr;
		static int s_recursion_count = 0;

		class recursion_count_updater {

		private:
			int* _recursion_count;

		public:
			recursion_count_updater(int* recursion_count)
				: _recursion_count(recursion_count) {

				(*_recursion_count)++;
			}

			~recursion_count_updater() {
				(*_recursion_count)--;
			}
		};
	}

	void set_assert_failed_handler(assert_failed_handler handler) {
		assert_impl::s_assert_failed_handler = handler;
	}

	bool handle_assert_failed(const char* file_name, int line_number, const char* expression) {
		
		//prevent handlers from potentially causing infinite recursion
		if (assert_impl::s_recursion_count != 0) {
			return true;;
		}

		assert_impl::recursion_count_updater updater(&assert_impl::s_recursion_count);
		
		if (assert_impl::s_assert_failed_handler != nullptr) {
			return assert_impl::s_assert_failed_handler(file_name, line_number, expression);
		}
		else {
			print_to_debug_output("-------\n");
			print_to_debug_output(build_string("ASSERT({0}) @ {1}:{2}\n", expression, file_name, line_number).c_str());
			print_to_debug_output("-------\n");
			return true;
		}
	}

}