#pragma once

#include "fixed_vector.h"

namespace solar {

	//scoped enumerations cannot be used as std::array size parameter without manual casting.
	//http://stackoverflow.com/questions/12927951/array-indexing-converting-to-integer-with-scoped-enumeration

	template <class ValueT, class EnumT>
	class enum_vector : public fixed_vector<ValueT, static_cast<std::size_t>(EnumT::count)> {
	};

}
