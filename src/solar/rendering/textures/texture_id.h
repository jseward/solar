#pragma once

#include <string>

#include "solar/archiving/single_value_archivable.h"
#include "solar/math/size.h"

#include "texture_create_params.h"

namespace solar {

	class texture;
	class texture_factory;
	class resource_system;

	class texture_id : public single_value_archivable {
	private:
		std::string _id;
		std::string _id_source_description;

		texture* _texture;

	public:
		texture_id();
		
		void set_id(const char* id, const char* id_source_description);

		void create_texture(texture_factory& texture_factory, resource_system& resource_system, const texture_create_params& params);
		void release_texture(texture_factory& texture_factory);
		texture* get_texture() const;
		size request_size() const;

		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;
	};

}