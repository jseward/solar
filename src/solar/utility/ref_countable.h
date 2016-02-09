#pragma once

#include "solar/utility/assert.h"

namespace solar {

	class ref_countable {
	private:
		int _ref_count;

	public:
		ref_countable()
			: _ref_count(0) {
		}
		
		void add_ref() {
			_ref_count++;
		}

		void sub_ref() {
			_ref_count--;
			ASSERT(_ref_count >= 0);
		}

		int get_ref_count() const {
			return _ref_count;
		}

		bool is_ref_count_zero() const {
			return (_ref_count == 0);
		}
	};

}