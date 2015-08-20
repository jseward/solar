#pragma once

#include "solar/utility/assert.h"

namespace solar {

	template<class ValueT, size_t MaxSize>
	class fixed_vector {
	public:
		enum { MAX_SIZE = MaxSize };

		typedef ValueT value_type;
		typedef size_t size_type;
		typedef ValueT* iterator;
		typedef const ValueT* const_iterator;

	private:
		size_t _size;
		ValueT _values[MAX_SIZE];

	public:
		fixed_vector()
			: _size(0) {
		}

		bool empty() const {
			return _size == 0;
		}

		bool full() const {
			return _size == MAX_SIZE;
		}

		size_t size() const {
			return _size;
		}

		const ValueT& at(size_t i) const {
			ASSERT(i >= 0);
			ASSERT(i < _size);
			return _values[i];
		}

		ValueT& at(size_t i) {
			ASSERT(i >= 0);
			ASSERT(i < _size);
			return _values[i];
		}

		const ValueT& operator[](size_t i) const {
			return at(i);
		}

		ValueT& operator[](size_t i) {
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

		void push_back(const ValueT& new_value) {
			push_back_unitialized() = new_value;
		}

		ValueT& push_back_unitialized()
		{
			ASSERT(_size < MAX_SIZE);
			_size++;
			return _values[_size - 1];
		}

		void clear() {
			_size = 0;
		}
	};

}