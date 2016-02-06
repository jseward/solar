#include "brush_set.h"

#include "brush.h"
#include "solar/resources/resource_system.h"
#include "solar/json/json_array.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	brush_set::brush_set(const resource_address& address)
		: _address(address) {
	}

	void brush_set::load(resource_system& resource_system) {
		resource_system.read_object_as_json(*this, _address);
	}

	brush* brush_set::get_brush_if_exists(const std::string& id) {
		auto iter = _brushes.find(id);
		if (iter != _brushes.end()) {
			return iter->second.get();
		}
		return nullptr;
	}

	void brush_set::read_from_archive(archive_reader& reader) {
		read_string(reader, "texture_pool_name", _texture_pool_name);

		reader.read_name("brushes");
		reader.read_array(
			[this](archive_reader&, unsigned int size) {
				_brushes.clear();
				_brushes.reserve(size);
				return true;
			},
			[this](archive_reader& reader, unsigned int index) {
				
				auto brush = new solar::brush(_texture_pool_name.c_str());
				read_object(reader, nullptr, *brush);

				if (brush->get_id().empty()) {
					reader.raise_error(build_string("brush has no id. {{ index:{} }}", index));
				}
				else if (_brushes.find(brush->get_id()) != _brushes.end()) {
					reader.raise_error(build_string("duplicate brush id found. {{ index:{} }}", index));
				}
				else {
					_brushes[brush->get_id()] = std::unique_ptr<solar::brush>(brush);
				}
			});
	}
	
	void brush_set::write_to_archive(archive_writer& writer) const {
		write_string(writer, "texture_pool_name", _texture_pool_name);

		auto iter = _brushes.begin();
		writer.write_name("brushes");
		writer.write_array(
			_brushes.size(),
			[&iter](archive_writer& writer, unsigned int) {
				write_object(writer, nullptr, *iter->second);
				iter++;
			});
	}

}