#pragma once

#include "solar/utility/assert.h"

namespace solar {

	//http://en.cppreference.com/w/cpp/experimental/optional

	//NOTE: no implict value operator is provided as it is too easy to write code with subtle bugs.
	//ex. 
	//optional<int> a;
	//optional<int> b;
	//return a < b; <- will compile but probably not what you want.

	template<typename T>
	class optional {
	private:
		bool _has_value;
		T _value;

	public:
		optional() 
			: _has_value(false) {
		}

		bool has_value() const {
			return _has_value;
		}

		optional<T>& operator=(const T& rhs) {
			_has_value = true;
			_value = rhs;
			return *this;
		}

		const T& value() const {
			ASSERT(_has_value);
			return _value;
		}

		T value_or_default() const {
			return _has_value ? _value : T();
		}

		T value_or(T&& default_value) const {
			return _has_value ? _value : default_value;
		}
	};

}