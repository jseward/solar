#pragma once

#include <algorithm>
#include <numeric>
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"

namespace solar {

	template<typename Con> void push_back_if_not_found(Con& dst, const typename Con::value_type& v) {
		if (std::find(dst.begin(), dst.end(), v) == dst.end()) {
			dst.push_back(v);
		}
	}

	template<typename Con> void push_back_and_verify_not_found(Con& dst, const typename Con::value_type& v) {
		IF_VERIFY(std::find(dst.begin(), dst.end(), v) == dst.end()) {
			dst.push_back(v);
		}
	}

	template<typename Con> void push_back_range(Con& dst, const Con& src) {
		dst.insert(dst.end(), src.begin(), src.end());
	}

	template<typename Con> void find_and_erase(Con& dst, const typename Con::value_type& v) {
		auto iter = std::find(dst.begin(), dst.end(), v);
		IF_VERIFY(iter != dst.end()) {
			dst.erase(iter);
		}
	}

	template<typename Con, typename Pr> void find_and_erase_if(Con& dst, Pr predicate) {
		auto iter = std::find_if(dst.begin(), dst.end(), predicate);
		IF_VERIFY(iter != dst.end()) {
			dst.erase(iter);
		}
	}

	template<typename Con> void erase_at_index(Con& dst, int index) {		
		ASSERT(index >= 0);
		ASSERT(index < uint_to_int(dst.size()));
		dst.erase(dst.begin() + index);
	}

	template<typename Con> bool try_find_and_erase(Con& dst, const typename Con::value_type& v) {
		auto iter = std::find(dst.begin(), dst.end(), v);
		if (iter != dst.end()) {
			dst.erase(iter);
			return true;
		}
		return false;
	}

	template<typename Con> bool any(const Con& in, const typename Con::value_type& v) {
		return (std::find(in.begin(), in.end(), v) != in.end());
	}

	template<typename Con, typename Pr> bool any_if(const Con& in, Pr predicate) {
		return (std::find_if(in.begin(), in.end(), predicate) != in.end());
	}

	template<typename Con, typename Pr> int count_if(const Con& in, Pr predicate) {
		return std::count_if(in.begin(), in.end(), predicate);
	}

	template<typename Con, typename BinaryOp> int accumulate(const Con& in, int init, BinaryOp binary_op) {
		return std::accumulate(in.begin(), in.end(), init, binary_op);
	}

	template<typename Con, typename Pr> typename Con::iterator find_if(Con& in, Pr predicate) {
		return std::find_if(in.begin(), in.end(), predicate);
	}

	template<typename Con, typename Pr> typename Con::const_iterator find_if(const Con& in, Pr predicate) {
		return std::find_if(in.begin(), in.end(), predicate);
	}

	template<typename Con> typename int find_index(const Con& in, const typename Con::value_type& v) {
		for (unsigned int i = 0; i < in.size(); ++i) {
			if (in[i] == v) {
				return uint_to_int(i);
			}
		}
		return -1;
	}

	template<typename Con, typename Pr> typename int find_index_if(const Con& in, Pr predicate) {
		for (unsigned int i = 0; i < in.size(); ++i) {
			if (predicate(in[i])) {
				return uint_to_int(i);
			}
		}
		return -1;
	}

	template<typename Con, typename Pr> Con copy_if(const Con& in, Pr predicate) {
		//http://www.cplusplus.com/reference/algorithm/copy_if
		Con out(in.size());
		auto it = std::copy_if(in.begin(), in.end(), out.begin(), predicate);
		out.resize(std::distance(out.begin(), it));  // shrink container to new size
		return out;
	}

	template<typename Con, typename Pr> void sort(Con& in, Pr predicate) {
		std::sort(in.begin(), in.end(), predicate);
	}

	template<typename Con, typename Pr> typename Con::value_type sort_and_get_first(const Con& in, Pr predicate) {
		Con sorted(in);
		std::sort(sorted.begin(), sorted.end(), predicate);
		IF_VERIFY(!sorted.empty()) {
			return *sorted.begin();
		}
		return Con::value_type();
	}

	template<typename Con, typename Pr> typename void remove_and_erase_if(Con& in, Pr predicate) {
		//http://stackoverflow.com/questions/17270837/stdvector-removing-elements-which-fulfill-some-conditions
		in.erase(std::remove_if(in.begin(), in.end(), predicate), in.end());
	}

}
