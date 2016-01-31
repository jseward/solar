#include "window_style_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/windows/components/button_style.h"
#include "solar/windows/components/label_style.h"
#include "solar/windows/components/text_entry_style.h"
#include "solar/windows/dialogs/dialog_window_style.h"

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
		ASSERT(_window_style_type_infos.empty());
	}

	void window_style_factory::add_all_common_types() {
		add_type<button_style>();
		add_type<label_style>();
		add_type<text_entry_style>();
		add_type<dialog_window_style>();
	}

	void window_style_factory::remove_all_common_types() {
		remove_type<button_style>();
		remove_type<label_style>();
		remove_type<text_entry_style>();
		remove_type<dialog_window_style>();
	}

	void window_style_factory::load_all() {
		for (auto& info : _window_style_type_infos) {
			auto address = _resource_system.resolve_address_to_file("window_style", "windows", ".window_style", info.first.c_str(), "window_style_factory::load()");
			if (!address.empty()) {
				_resource_system.read_object_as_json(*info.second.get(), address);
				_resource_system.begin_watching_resource(this, address, info.second.get());
			}
		}
		_caching_context.increment();
		_is_loaded = true;
	}

	void window_style_factory::unload_all() {
		_resource_system.end_watching_resources(this);
		_is_loaded = false;
	}

	const resource_factory_caching_context& window_style_factory::get_caching_context() const {
		return _caching_context;
	}

	void window_style_factory::on_file_changed(const std::string& path, void* data) {
		window_style_type_info* info = reinterpret_cast<window_style_type_info*>(data);
		_resource_system.read_object_as_json(*info, make_resource_address_with_file_system_provider(true, path));
		_caching_context.increment();
	}

}
