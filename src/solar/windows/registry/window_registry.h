#pragma once

#include <vector>
#include <memory>

namespace solar {

	class resource_system;
	class window;
	class window_registry_window_collection;

	class window_registry {
	private:
		using window_collection_vector = std::vector<std::unique_ptr<window_registry_window_collection>>;

	private:
		resource_system& _resource_system;
		window_collection_vector _window_collections;

	public:
		window_registry(resource_system& resource_system);
		~window_registry();
		window_registry(const window_registry&) = delete;
		window_registry& operator=(const window_registry&) = delete;

		void add_window(window* window);
		void remove_window(window* window);

		void load();
		void save();

	private:
		window_collection_vector::iterator find_window_collection(window* window);
	};

}