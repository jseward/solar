#include "setting_registry.h"

#include "solar/io/file_system.h"
#include "solar/io/file_change_watcher.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	const char* setting_registry::USER_FILE_FOLDER_NAME = "settings";

	setting_registry::setting_registry(file_system& file_system, file_change_watcher& file_change_watcher)
		: _file_system(file_system)
		, _file_change_watcher(file_change_watcher)
		, _can_add_settings(true) {
	}

	setting_registry::~setting_registry() {
	}

	std::string setting_registry::get_root_dir_path() const {
		return _file_system.make_user_file_dir_path(USER_FILE_FOLDER_NAME);
	}

	std::string setting_registry::get_group_file_path(const std::string& group_name) const {
		return _file_system.make_user_file_path(USER_FILE_FOLDER_NAME, group_name + ".setting");
	}

	void setting_registry::add_setting(std::shared_ptr<setting> setting) {
		ASSERT(_can_add_settings);
		auto group = find_group_by_name(setting->get_group_name());
		if (group == nullptr) {
			group = add_group(setting->get_group_name());
		}
		
		group->add_setting(setting);
	}

	setting_group* setting_registry::find_group_by_name(const std::string& group_name) {
		auto group_iter = find_if(_setting_groups, [&group_name](const std::unique_ptr<setting_group>& setting_group) { 
			return setting_group->get_group_name() == group_name; 
		});
		if (group_iter != _setting_groups.end()) {
			return group_iter->get();
		}
		return nullptr;
	}

	setting_group* setting_registry::add_group(const std::string& group_name) {
		auto new_group = std::make_unique<setting_group>(group_name);
		auto new_group_ptr = new_group.get();
		_setting_groups.push_back(std::move(new_group));
		return new_group_ptr;
	}

	void setting_registry::load_all_settings() {
		_can_add_settings = false; //after loading for the first time don't allow adding more settings. would just get confusing and error prone.
		for (auto& group : _setting_groups) {
			auto path = get_group_file_path(group->get_group_name());
			group->load(_file_system, path);
			_file_change_watcher.begin_watching_file(this, path, group.get());
		}
	}

	void setting_registry::teardown() {
		_file_change_watcher.end_watching_files(this);
	}

	void setting_registry::on_file_changed(const std::string& path, void* data) {
		setting_group* group = reinterpret_cast<setting_group*>(data);
		ASSERT(get_group_file_path(group->get_group_name()) == path);
		group->load(_file_system, path);
	}

}