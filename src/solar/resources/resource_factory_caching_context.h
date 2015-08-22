#pragma once

#include "resource_id_caching_context.h"

namespace solar {

	class resource_factory_caching_context {
	private:
		int _value;

	public:
		resource_factory_caching_context()
			: _value(0) {
		}

		int get_value() const {
			return _value;
		}

		void increment() {
			_value++;
		}
	};

}