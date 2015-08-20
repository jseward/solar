#pragma once

#include <vector>
#include <memory>
#include <string>
#include "brush_set.h" //can't forward declare if in std::vector

namespace solar {

	class brush;
	class resource_system;

	class brush_factory {
	private:
		resource_system& _resource_system;
		int _brush_caching_context;
		std::vector<std::unique_ptr<brush_set>> _brush_sets;
		std::unique_ptr<brush> _empty_brush;

	public:
		brush_factory(resource_system& resource_system);
		void setup();
		void teardown();
		int get_brush_caching_context() const;
		brush* get_brush(const std::string& id, const std::string& id_source_description);
	};

}