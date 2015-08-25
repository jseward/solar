#pragma once

#include "solar/utility/assert.h"
#include "solar/math/point.h"

namespace solar {

	template<int Width, int Height>
	class fixed_grid_coord_iterator {
	private:
		point _coord;

	public:
		fixed_grid_coord_iterator(const point& coord) 
			: _coord(coord) {
		}

		bool operator!=(const fixed_grid_coord_iterator& other) const {
			return _coord != other._coord;
		}

		const point& operator*() const {
			return _coord;
		}

		const fixed_grid_coord_iterator& operator++() {
			_coord._x++;
			if (_coord._x >= Width) {
				_coord._x = 0;
				_coord._y++;
				ASSERT(_coord._y <= Height);
			}
			return *this;
		}
	};

	template<int Width, int Height>
	class fixed_grid_coords {
	public:
		using iterator = fixed_grid_coord_iterator<Width, Height>;

	public:
		iterator begin() const {
			return iterator(point(0, 0));
		}

		iterator end() const {
			return iterator(point(0, Height));
		}
	};

	template<typename ValueT, int Width, int Height>
	class fixed_grid {
	public:
		using coord_iterator = fixed_grid_coord_iterator<Width, Height>;
		using coords = fixed_grid_coords<Width, Height>;
		using reference = ValueT&;
		using const_reference = const ValueT&;
		using iterator = ValueT*;
		using const_iterator = const ValueT*;

	public:
		enum { VALUE_COUNT = Width * Height };

	private:
		ValueT _values[VALUE_COUNT];

	public:
		fixed_grid() {
		}

		fixed_grid(const ValueT& initial_value) {
			fill(initial_value);
		}

		void fill(const ValueT& fill_value) {
			for (int i = 0; i < VALUE_COUNT; ++i) {
				_values[i] = fill_value;
			}
		}

		coords get_coords() const {
			return coords();
		}

		reference at_index(int index) {
			ASSERT(index >= 0);
			ASSERT(index < VALUE_COUNT);
			return _values[index];
		}

		reference at(int x, int y) {
			return at_index(coord_to_index(x, y));
		}

		reference at(const point& coord) {
			return at_index(coord_to_index(coord._x, coord._y));
		}

		const_reference at_index(int index) const {
			ASSERT(index >= 0);
			ASSERT(index < VALUE_COUNT);
			return _values[index];
		}

		const_reference at(int x, int y) const {
			return at_index(coord_to_index(x, y));
		}

		const_reference at(const point& coord) const {
			return at_index(coord_to_index(coord._x, coord._y));
		}

		iterator begin() {
			return _values;
		}

		iterator end() {
			return _values + VALUE_COUNT;
		}

		const_iterator begin() const {
			return _values;
		}

		const_iterator end() const {
			return _values + VALUE_COUNT;
		}

	public:
		static int coord_to_index(int x, int y) {
			return x + (Width * y);
		}

		static int coord_to_index(const point& coord) {
			return coord_to_index(coord._x, coord._y);
		}
	};

}