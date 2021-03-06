#pragma once

#include <array>

namespace solar {

	//scoped enumerations cannot be used as std::array size parameter without manual casting.
	//http://stackoverflow.com/questions/12927951/array-indexing-converting-to-integer-with-scoped-enumeration

	template <class ValueT, class EnumT>
	class enum_array : public std::array<ValueT, static_cast<std::size_t>(EnumT::count)> {
	public:
		typedef EnumT enum_type;

	public:
		bool is_enum_valid(EnumT e) const {
			return 
				(static_cast<size_t>(e) >= 0) && 
				(static_cast<size_t>(e) < static_cast<size_t>(EnumT::count));
		}

		ValueT& at(EnumT e) {
			return array::at(static_cast<size_t>(e));
		}

		const ValueT& at(EnumT e) const {
			return array::at(static_cast<size_t>(e));
		}

		ValueT& operator[](EnumT e) {
			return array::at(static_cast<size_t>(e));
		}

		const ValueT& operator[](EnumT e) const {
			return array::at(static_cast<size_t>(e));
		}
	};

}
