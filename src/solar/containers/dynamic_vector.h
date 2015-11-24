#pragma once

namespace solar {

	//most code should use std::vector, but dynamic_vector has some differences:
	//clear_fast_no_destruct() - clears vector without calling destructor
	//push_back_no_construct() - faster push_back without calling constructor

	template<class ValueT>
	class dynamic_vector {
	public:
		typedef ValueT value_type;
		typedef size_t size_type;
		typedef ValueT* iterator;
		typedef const ValueT* const_iterator;

	private:
		size_t _max_size;
		size_t _size;
		value_type* _values; //todo - convert this to raw uint8_t* and reinterpret_cast everywhere. allows to construct dynamic_vector of types with no default constructor!

	public:
		dynamic_vector()
			: _max_size(0)
			, _size(0)
			, _values(nullptr) {
		}

		dynamic_vector(const dynamic_vector&) = delete;
		dynamic_vector& operator=(const dynamic_vector&) = delete;

		~dynamic_vector() {
			delete[] _values;
		}

		void reserve(size_t new_size, bool preserve_data = false) {
			ASSERT(new_size >= 0);
			if (new_size != _max_size) {
				value_type* new_values = nullptr;
				if (new_size > 0) {
					new_values = new value_type[new_size];
					if (preserve_data) {
						ASSERT(new_size > _max_size); //impossible to preserve is we are shrinking
						for (unsigned int i = 0; i < _size; ++i) {
							new_values[i] = _values[i];
						}
					}
				}

				delete[] _values;
				_values = new_values;
				_max_size = new_size;
			}

			if (!preserve_data) {
				_size = 0;
			}
		}

		const value_type* data() const {
			return _values;
		}

		size_t size() const {
			return _size;
		}

		void clear() {
			for (unsigned int i = 0; i < _size; ++i) {
				_values[i].~value_type();
			}
			_size = 0;
		}

		void clear_fast_no_destruct() {
			//check std::is_trivially_destructible<ValueT>? need more research as can't get it to compile to pick correct clear call based on that trait...
			_size = 0;
		}

		template<class... Args> void emplace_back(Args&&... args) {
			ASSERT(_size < _max_size);
			_size++;
			_values[_size - 1] = value_type(args...);
		}

		void push_back(const ValueT& new_value) {
			push_back_no_construct() = new_value;
		}

		ValueT& push_back_no_construct() {
			ASSERT(_size < _max_size);
			_size++;
			return _values[_size - 1];
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
