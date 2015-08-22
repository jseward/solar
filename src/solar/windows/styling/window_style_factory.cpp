#include "window_style_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_style_factory::window_style_type_info::window_style_type_info(std::unique_ptr<window_style> style_def) 
		: _style_def(std::move(style_def)) {
	}

	void window_style_factory::window_style_type_info::read_from_archive(archive_reader& reader) {
		_styles.clear();
		reader.read_objects(
			"styles",
			[&](int) {},
			[&](archive_reader& reader, unsigned int) {
				std::string id;
				read_string(reader, "id", id);
				if (_styles.find(id) != _styles.end()) {
					reader.raise_error(build_string("duplicate window_style found: {}", id));
				}
				auto style = _style_def->clone();
				style->read_from_archive(reader);
				_styles[id] = std::move(style);
			});
	}

	void window_style_factory::window_style_type_info::write_to_archive(archive_writer& writer) const {
		ASSERT(false);
		UNUSED_PARAMETER(writer);
	}

	window_style_factory::window_style_factory(resource_system& resource_system) 
		: _resource_system(resource_system) {
	}

	window_style_factory::~window_style_factory() {
	}

	void window_style_factory::load() {
		for (auto& info : _window_style_type_infos) {
			auto address = _resource_system.resolve_address("window_style", "windows", ".window_style", info.first.c_str(), "window_style_factory::load()");
			if (!address.empty()) {
				_resource_system.read_object_as_json(*info.second.get(), address);
			}
		}
		_caching_context.increment();
	}

	const resource_factory_caching_context& window_style_factory::get_caching_context() const {
		return _caching_context;
	}

}