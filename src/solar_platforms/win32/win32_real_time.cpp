#include "solar/time/real_time.h"
#include "win32_headers.h"
#include "solar/utility/verify.h"

namespace solar {

	namespace win32_real_time_impl {
		class time_period_sentinel {
		public:
			unsigned long _base_time;

		public:
			time_period_sentinel() {
				VERIFY(::timeBeginPeriod(1U) == TIMERR_NOERROR);
				_base_time = ::timeGetTime();
			}

			~time_period_sentinel() {
				VERIFY(::timeEndPeriod(1U) == TIMERR_NOERROR);
			}
		};
	}

	unsigned long get_real_time_in_ms() {
		static win32_real_time_impl::time_period_sentinel sentinel;
		return ::timeGetTime() - sentinel._base_time;
	}

}
