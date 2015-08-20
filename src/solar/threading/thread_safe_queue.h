#pragma once

#include <queue>
#include <mutex>

namespace solar {

	template<typename T>
	class thread_safe_queue {
	private:
		std::queue<T> _queue;
		std::mutex _mutex;

	public:
		template<class... Args> void emplace(Args&&... args) {
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.emplace(args...);
		}

		bool try_pop(T& out) {
			if (!_queue.empty()) {
				std::lock_guard<std::mutex> lock(_mutex);
				if (!_queue.empty()) {
					out = std::move(_queue.front());
					_queue.pop();
					return true;
				}
			}
			return false;
		}

		void clear() {
			std::lock_guard<std::mutex> lock(_mutex);
			_queue = {};
		}
	};

}