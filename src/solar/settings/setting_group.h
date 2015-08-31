#pragma once

#include <vector>
#include <memory>
#include "setting.h"
#include "solar/archiving/archivable.h"

namespace solar {

	class file_system;

	class setting_group : archivable {
	public:
		static const char* USER_FILE_FOLDER_NAME;

	private:
		std::string _group_name;
		bool _has_changes;
		std::vector< std::shared_ptr<setting> > _settings;

	public:
		explicit setting_group(const std::string& group_name);
		~setting_group();

		bool empty() const;
		const std::string& get_group_name() const;

		void add_setting(std::shared_ptr<setting> setting);
		void remove_setting(std::shared_ptr<setting> setting);

		void load(file_system& file_system);

		std::string get_file_path(file_system& file_system) const;

	private:
		int get_archive_version() const;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}