#pragma once

#include <string>

namespace solar {

	class file_system;
	class archivable;

	extern void load_auto_versioned_json_archive(file_system& file_system, const std::string& path, int version, archivable& object);
	extern void save_auto_versioned_json_archive(file_system& file_system, const std::string& path, int version, const archivable& object);

}