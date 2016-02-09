#pragma once

#include <vector>
#include <memory>
#include <string>
#include "solar/resources/resource_factory_caching_context.h"
#include "brush_set.h" //can't forward declare if in std::vector

namespace solar {

	class brush;
	class texture_factory;
	class resource_system;

	class brush_factory {
	private:
		texture_factory& _texture_factory;
		resource_system& _resource_system;
		resource_factory_caching_context _caching_context;
		std::vector<std::unique_ptr<brush_set>> _brush_sets;
		std::unique_ptr<brush> _empty_brush;

	public:
		brush_factory(texture_factory& texture_factory, resource_system& resource_system);

		void setup();
		void teardown();

		const resource_factory_caching_context& get_caching_context() const;
		brush* get_brush(const std::string& id, const std::string& id_source_description);
	};

}