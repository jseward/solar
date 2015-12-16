#include "font_factory.h"
#include "solar/utility/assert.h"
#include "solar/resources/resource_id_manifest.h"

namespace solar {

	font_factory::font_factory(resource_system& resource_system)
		: _resource_system(resource_system) {
	}

	void font_factory::setup(const char* texture_pool_name) {
		ASSERT(_fonts.empty());

		resource_id_manifest manifest;
		manifest.load(_resource_system, "font_manifest", "fonts", ".font_manifest", "_font_manifest", "font_factory");

		for (auto id : manifest.get_ids()) {
			auto address = _resource_system.resolve_address_to_file("font", "fonts", ".font", id.c_str(), manifest.get_address().to_string().c_str());
			_fonts[id] = std::make_unique<font>(address);
		}

		for (auto& font : _fonts) {
			font.second->load(_resource_system, texture_pool_name);
		}

		_caching_context.increment();
	}

	void font_factory::teardown() {
	}

	font* font_factory::get_font(const std::string& id, const std::string& id_source_description) {
		auto iter = _fonts.find(id);
		if (iter != _fonts.end()) {
			return iter->second.get();
		}

		ALERT("font not found.\n\nid : {0}\nid_source_description : {1}", id, id_source_description);
		if (_empty_font == nullptr) {
			_empty_font = std::make_unique<font>(resource_address());
		}
		return _empty_font.get();
	}

	const resource_factory_caching_context& font_factory::get_caching_context() const {
		return _caching_context;
	}

}