#include "texture_id.h"

#include "solar/resources/resource_system.h"
#include "solar/archiving/archive_reader.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/utility/verify.h"

#include "texture_factory.h"

namespace solar {

	texture_id::texture_id() {
	}

	void texture_id::set_id(const char* id, const char* id_source_description) {
		ASSERT(_texture == nullptr);
		_id = id;
		_id_source_description = id_source_description;
	}

	void texture_id::create_texture(texture_factory& texture_factory, resource_system& resource_system, const texture_create_params& params) {
		ASSERT(_texture == nullptr);
		
		auto address = resource_system.resolve_address_to_file("texture", "textures", ".dds;.tga", _id.c_str(), _id_source_description.c_str());
		if (!address.empty()) {
			_texture = texture_factory.create_texture(address, params);
		}
	}

	void texture_id::release_texture(texture_factory& texture_factory) {
		if (_texture != nullptr) {
			texture_factory.release_texture(_texture);
			_texture = nullptr;
		}
	}

	texture* texture_id::get_texture() const {
		return _texture;
	}

	size texture_id::request_size() const {
		if (_texture != nullptr) {
			return _texture->request_size();
		}
		return size();
	}

	void texture_id::read_from_archive(archive_reader& reader, const char* name) {
		read_string(reader, name, _id);
		_id_source_description = reader.get_source_description();
	}

	void texture_id::write_to_archive(archive_writer& writer, const char* name) const {
		write_string(writer, name, _id);
	}

}