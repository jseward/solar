#include "text_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"

namespace solar {


	text_factory::text_factory(resource_system& resource_system) 
		: _resource_system(resource_system) {
	}

	void text_factory::setup(const char* language) {
		ASSERT(_language.empty());
		ASSERT(_language_address.empty());
		ASSERT(_text_map.empty());

		_language = language;
		_language_address = _resource_system.resolve_address_to_file("text", "text", ".text", language, "text_factory::setup");
		if (!_language_address.empty()) {
			load_from_address(_language_address);
			_resource_system.begin_watching_resource(this, _language_address);
		}
	}

	void text_factory::teardown() {
		_resource_system.end_watching_resources(this);
		_language = "";
		_text_map.clear();
		_caching_context.increment();
	}

	void text_factory::load_from_address(const resource_address& address) {
		auto stream = _resource_system.open_stream_to_read(address);
		if (stream != nullptr) {
			_text_map.load_from_stream(*stream);
			_resource_system.close_stream(stream);
		}
		_caching_context.increment();
	}

	const resource_factory_caching_context& text_factory::get_caching_context() const {
		return _caching_context;
	}

	text* text_factory::get_text(const std::string& id, const std::string& id_source_description) {
		auto iter = _text_map.find(id);
		if (iter != _text_map.end()) {
			return iter->second.get();
		}

		ALERT("text not found.\n\nid : {0}\nid_source_description : {1}", id, id_source_description);
		if (_empty_text == nullptr) {
			_empty_text = std::make_unique<text>(L"???");
		}
		return _empty_text.get();
	}

	void text_factory::on_file_changed(const std::string& path, void* data) {
		ASSERT(data == nullptr);
		ASSERT(path == _language_address.get_path());
		load_from_address(_language_address);
	}

}