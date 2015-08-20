#pragma once

#include <unordered_map>
#include <memory>
#include "solar/resources/resource_address.h"
#include "solar/archiving/archivable.h"

namespace solar {

	class brush;
	class resource_system;

	class brush_set : public archivable {
	private:
		resource_address _address;
		std::string _texture_pool_name;
		std::unordered_map<std::string, std::unique_ptr<brush>> _brushes;

	public:
		brush_set(const resource_address& address);
		void load(resource_system& resource_system);
		brush* get_brush_if_exists(const std::string& id);

	private:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}