#include "setting_group.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/checksum.h"
#include "solar/io/file_system.h"
#include "solar/archiving/auto_versioned_json_archive_helpers.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	setting_group::setting_group(const std::string& group_name) 
		: _group_name(group_name)
		, _has_changes(false) {
	}

	setting_group::~setting_group() {
	}

	bool setting_group::empty() const {
		return _settings.empty();
	}

	const std::string& setting_group::get_group_name() const {
		return _group_name;
	}

	void setting_group::add_setting(std::shared_ptr<setting> setting) {
		push_back_and_verify_not_found(_settings, setting);
	}

	void setting_group::remove_setting(std::shared_ptr<setting> setting) {
		find_and_erase(_settings, setting);
	}

	void setting_group::load(file_system& file_system) {
		load_auto_versioned_json_archive(file_system, get_file_path(file_system), get_archive_version(), *this);
	}

	std::string setting_group::get_file_path(file_system& file_system) const {
		return file_system.make_user_file_path("settings", _group_name + ".setting");
	}

	int setting_group::get_archive_version() const {
		return checksum().add_container(_settings, [](std::shared_ptr<setting> s) {
			return s->get_archive_version_checksum();
		}).to_int();
	}

	void setting_group::read_from_archive(archive_reader& reader) {
		for (auto& setting : _settings) {
			read_object(reader, setting->get_name().c_str(), *setting.get());
		}
	}
	
	void setting_group::write_to_archive(archive_writer& writer) const {
		for (auto& setting : _settings) {
			write_object(writer, setting->get_name().c_str(), *setting.get());
		}
	}

}