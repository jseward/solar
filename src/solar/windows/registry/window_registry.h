#pragma once

#include <vector>
#include <memory>
#include "solar/io/file_change_handler.h"
#include "solar/resources/resource_address.h"

namespace solar {

	class resource_system;
	class window;
	class window_registry_window_collection;

	class window_registry : public file_change_handler {
	private:
		using window_collection_vector = std::vector<std::unique_ptr<window_registry_window_collection>>;

	private:
		resource_system& _resource_system;
		window_collection_vector _window_collections;
		bool _has_loaded_all;

	public:
		window_registry(resource_system& resource_system);
		window_registry(const window_registry&) = delete;
		window_registry& operator=(const window_registry&) = delete;
		virtual ~window_registry();

		void add_window(window* window);
		void remove_window(window* window);

		void load_all();
		void teardown();

	private:
		window_collection_vector::iterator find_window_collection(window* window);
		resource_address resolve_window_collection_address(const window_registry_window_collection& collection);

	private:
		virtual void on_file_changed(const std::string& path, void* data) override;
	};

}