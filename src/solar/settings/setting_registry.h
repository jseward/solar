#pragma once

#include <vector>
#include <memory>
#include <string>
#include "setting_group.h"

namespace solar {

	class file_system;

	class setting_registry {

	private:
		file_system& _file_system;

		bool _can_add_settings;
		std::vector< std::unique_ptr<setting_group> > _setting_groups;

	public:
		setting_registry(file_system& file_system);
		~setting_registry();

		void add_setting(std::shared_ptr<setting> setting);

		void load_all_settings();

	private:
		setting_group* find_group_by_name(const std::string& group_name);
		setting_group* add_group(const std::string& group_name);
	};

}