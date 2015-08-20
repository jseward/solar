#pragma once

#include "solar/utility/assert.h"

namespace solar {

	template<typename T>
	class camera_cached_value {
	private:
		int _last_cached_context;
		T _value;

	public:
		camera_cached_value() 
			: _last_cached_context(-1)
			, _value() {
		}

		const T& get(int current_context) const {
			ASSERT(_last_cached_context == current_context);
			return _value;
		}

		void set(int current_context, const T& new_value) {
			_last_cached_context = current_context;
			_value = new_value;
		}

		bool is_cached(int current_context) const {
			return (_last_cached_context == current_context);
		}
	};

}