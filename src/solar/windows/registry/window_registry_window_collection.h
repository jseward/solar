#pragma once

#include <vector>
#include "solar/resources/resource_address.h"

namespace solar {

	class resource_system;
	class window;

	class window_registry_window_collection {
	private:
		resource_system& _resource_system;
		std::string _id;
		std::vector<window*> _windows;

	public:
		window_registry_window_collection(resource_system& resource_system, window* window);
		~window_registry_window_collection();
		window_registry_window_collection(const window_registry_window_collection&) = delete;
		window_registry_window_collection& operator=(const window_registry_window_collection&) = delete;

		const std::string& get_id() const;
		bool empty() const;

		void add_window(window* window);
		void remove_window(window* window);

		void load();
		void save();

	private:
		static void handle_loaded_recursive(window& window);
	};

}