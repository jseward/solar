#include "auto_versioned_json_archive_helpers.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/io/file_system.h"
#include "json_archive_reader.h"
#include "json_archive_writer.h"
#include "archivable.h"
#include "archiving_helpers.h"

namespace solar {

	void load_auto_versioned_json_archive(file_system& file_system, const std::string& path, int version, archivable& object) {
		if (!file_system.does_file_exist(path)) {
			TRACE("auto_versioned_json_archive file does not exist : {}", path);
			save_auto_versioned_json_archive(file_system, path, version, object);
		}

		std::unique_ptr<json_archive_reader> reader;

		auto fs = file_system.open_file_stream(path, file_mode::OPEN_READ);
		if (fs != nullptr) {
				
			reader = std::make_unique<json_archive_reader>(*fs);
			int file_version = 0;
			read_int(*reader, "version", file_version);

			if (file_version != version) {
				TRACE("auto_versioned_json_archive version does not match {{ expected_version:{} , found_version:{} , file:'{}' }}", version, file_version, path);
				
				reader.reset();
				file_system.close_file_stream(fs);
				fs = nullptr;

				save_auto_versioned_json_archive(file_system, path, version, object);
					
				fs = file_system.open_file_stream(path, file_mode::OPEN_READ);
				if (fs != nullptr) {
					reader = std::make_unique<json_archive_reader>(*fs);
					int saved_file_version = 0;
					read_int(*reader, "version", saved_file_version);
					ASSERT(saved_file_version == version);
				}
			}
		}

		if (reader) {
			object.read_from_archive(*reader);
		}

		if (fs != nullptr) {
			file_system.close_file_stream(fs);
		}
	}

	void save_auto_versioned_json_archive(file_system& file_system, const std::string& path, int version, const archivable& object) {
		auto fs = file_system.open_file_stream(path, file_mode::CREATE_WRITE);
		if (fs != nullptr) {
			json_archive_writer writer(*fs);
			writer.begin_writing();
			write_int(writer, "version", version);
			object.write_to_archive(writer);
			writer.end_writing();
			file_system.close_file_stream(fs);
		}
	}

}