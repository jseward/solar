#pragma once

#include <vector>
#include <functional>

namespace solar {

	template<typename ...A>
	class delegate {
	private:
		std::vector<std::function<void(A...)>> _functions;

	public:
		void bind(std::function<void(A...)> f) {
			_functions.push_back(f);
		}

		void operator()(A...) {
			for (auto& f : _functions) {
				f(A...);
			}
		}
	};

}