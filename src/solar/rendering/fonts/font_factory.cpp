#include "font_factory.h"

#include "solar/utility/assert.h"
#include "solar/resources/resource_id_manifest.h"
#include "solar/resources/resource_system.h"

namespace solar {

	font_factory::font_factory(texture_factory& texture_factory, resource_system& resource_system)
		: _texture_factory(texture_factory)
		, _resource_system(resource_system) {
	}

	void font_factory::setup() {
		ASSERT(_fonts.empty());

		resource_id_manifest manifest;
		manifest.load(_resource_system, "font_manifest", "fonts", ".font_manifest", "_font_manifest", "font_factory");

		for (auto id : manifest.get_ids()) {
			auto address = _resource_system.resolve_address_to_file("font", "fonts", ".font", id.c_str(), manifest.get_address().to_string().c_str());
			_fonts[id] = std::make_unique<font>(address);
		}

		for (auto& font : _fonts) {
			font.second->load(_resource_system);
			font.second->create_textures(_texture_factory, _resource_system);
		}

		_caching_context.increment();
	}

	void font_factory::teardown() {
		for (auto& font : _fonts) {
			font.second->release_textures(_texture_factory);
		}

		_fonts.clear();

		_caching_context.increment();
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