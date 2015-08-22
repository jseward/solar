#pragma once

namespace solar {

	class resource_id_caching_context {
	private:
		int _value;

	public:
		class resource_id_caching_context()
			: _value(-1) {
		}

		void reset() {
			_value = -1;
		}

		int get_value() const {
			return _value;
		}

		void set_value(int new_value) {
			_value = new_value;
		}
	};

}