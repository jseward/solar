#pragma once

#include <vector>
#include <memory>
#include <string>
#include "setting_group.h"
#include "solar/io/file_change_handler.h"

namespace solar {

	class file_system;
	class file_change_watcher;

	class setting_registry : public file_change_handler {
	private:
		static const char* USER_FILE_FOLDER_NAME;

	private:
		file_system& _file_system;
		file_change_watcher& _file_change_watcher;

		bool _can_add_settings;
		std::vector<std::unique_ptr<setting_group>> _setting_groups;

	public:
		setting_registry(file_system& file_system, file_change_watcher& file_change_watcher);
		setting_registry(const setting_registry&) = delete;
		setting_registry& operator=(const setting_registry&) = delete;
		~setting_registry();

		std::string get_root_dir_path() const;

		//NOTE: no setup() as setup involves all systems adding their own settings then load_all_settings being called.
		void add_setting(std::shared_ptr<setting> setting);
		void load_all_settings();
		void teardown();

	private:
		std::string get_group_file_path(const std::string& group_name) const;
		setting_group* find_group_by_name(const std::string& group_name);
		setting_group* add_group(const std::string& group_name);

	private:
		virtual void on_file_changed(const std::string& path, void* data) override;
	};

}