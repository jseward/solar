#pragma once

namespace solar {

	template<class ValueT>
	class dynamic_array {
	public:
		typedef ValueT value_type;
		typedef size_t size_type;
		typedef ValueT* iterator;
		typedef const ValueT* const_iterator;

	private:
		size_t _size;
		value_type* _values;

	public:
		dynamic_array()
			: _size(0)
			, _values(nullptr) {
		}

		explicit dynamic_array(size_t size)
			: _size(size)
			, _values(nullptr) {

			ASSERT(size > 0);
			_values = new value_type[size];
		}

		dynamic_array(size_t size, const value_type& initial_fill_value)
			: _size(size)
			, _values(nullptr) {

			ASSERT(size != 0);
			_values = new value_type[size];
			fill(initial_fill_value);
		}

		~dynamic_array() {
			delete [] _values;
		}

		void reserve(size_t new_size) {
			ASSERT(_size == 0);
			ASSERT(new_size > 0);
			_size = new_size;
			_values = new value_type[new_size];
		}

		size_t size() const {
			return _size;
		}

		void fill(const value_type& fill_value) {
			for (unsigned int i = 0; i < _size; ++i) {
				_values[i] = fill_value;
			}
		}

		const value_type& at(size_t i) const {
			ASSERT(i >= 0);
			ASSERT(i < _size);
			return _values[i];
		}

		value_type& at(size_t i) {
			ASSERT(i >= 0);
			ASSERT(i < _size);
			return _values[i];
		}

		const value_type& operator[](size_t i) const {
			return at(i);
		}

		value_type& operator[](size_t i) {
			return at(i);
		}

		iterator begin() {
			return _values;
		}

		iterator end() {
			return (_values + _size);
		}

		const_iterator begin() const {
			return _values;
		}

		const_iterator end() const {
			return (_values + _size);
		}
	};

}