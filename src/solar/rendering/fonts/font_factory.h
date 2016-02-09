#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "font.h"
#include "solar/resources/resource_factory_caching_context.h"

namespace solar {

	class texture_factory;
	class resource_system;

	class font_factory {
	private:
		texture_factory& _texture_factory;
		resource_system& _resource_system;

		resource_factory_caching_context _caching_context;
		std::unordered_map<std::string, std::unique_ptr<font>> _fonts;
		std::unique_ptr<font> _empty_font;

	public:
		font_factory(texture_factory& texture_factory, resource_system& resource_system);

		void setup();
		void teardown();

		const resource_factory_caching_context& get_caching_context() const;
		font* get_font(const std::string& id, const std::string& id_source_description);
	};

}