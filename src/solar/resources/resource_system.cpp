#include "resource_system.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/trace.h"
#include "solar/utility/verify.h"
#include "solar/containers/container_helpers.h"
#include "solar/strings/string_convert.h"
#include "solar/strings/string_helpers.h"
#include "solar/archiving/archivable.h"
#include "solar/archiving/json_archive_reader.h"
#include "solar/archiving/json_archive_writer.h"
#include "solar/io/file_system.h"
#include "solar/io/file_path_helpers.h"
#include "solar/io/file_change_watcher.h"
#include "solar/io/file_change_handler.h"

namespace solar {

	resource_system::resource_system(file_system& file_system, file_change_watcher& file_change_watcher)
		: _file_system(file_system)
		, _file_change_watcher(file_change_watcher) {
	}

	resource_system::~resource_system() {
		ASSERT(_open_file_system_streams.empty());
	}

	void resource_system::setup(const resource_system_params& params) {
		TRACE("resource_system setup {{ provider_count:{} , is_watching_enabled:{} }}", params._providers.size(), bool_to_string(params._is_watching_enabled));
		for (const auto& provider : params._providers) {
			TRACE("resource_system provider : {}", provider.get_description());
		}

		_providers = params._providers;
	}

	void resource_system::teardown() {
	}

	std::vector<std::string> resource_system::get_all_file_system_provider_dir_paths() const {
		std::vector<std::string> dir_paths;
		for (const auto& provider : _providers) {
			if (provider.get_type() == resource_provider_type::FILE_SYSTEM) {
				dir_paths.push_back(provider.get_root_path());
			}
		}
		return dir_paths;
	}

	resource_address resource_system::resolve_address_to_file(
		const char* resource_type_name,
		const char* folder,
		const char* extensions,
		const char* id,
		const char* id_source_description) {

		return resolve_address(true, resource_type_name, folder, extensions, id, id_source_description);
	}

	resource_address resource_system::resolve_address_to_directory(
		const char* resource_type_name,
		const char* folder,
		const char* id,
		const char* id_source_description) {

		return resolve_address(false, resource_type_name, folder, "", id, id_source_description);
	}

	resource_address resource_system::resolve_address(
		bool is_file, 
		const char* resource_type_name, 
		const char* folder, 
		const char* extensions, 
		const char* id, 
		const char* id_source_description) {

		if (is_string_empty(id)) {
			ALERT("resource_id is empty.\n\nresource_type: {}\nid_source: {}", resource_type_name, id_source_description);
			return resource_address();
		}

		if (does_id_have_bad_chars(id)) {
			ALERT("resource_id can only contain lowercase alphanumeric chars and _.\n\nresource_type: '{}'\nresource_id: '{}'\nid_source: {}", resource_type_name, id, id_source_description);
			return resource_address();
		}

		for (auto& provider : _providers) {
			auto address = resolve_address_with_provider(is_file, folder, extensions, id, provider);
			if (!address.empty()) {
				return address;
			}
		}

		ALERT("resource not found : {}{}\n\nresource_type: {}\nfolder: {}\nid_source: {}\n\nproviders: {}", id, extensions, resource_type_name, folder, id_source_description, build_newline_seperated_providers_description());
		return resource_address();
	}

	resource_address resource_system::resolve_address_with_provider(bool is_file, const char* folder, const char* extensions, const char* id, const resource_provider& provider) {
		if (provider.get_type() == resource_provider_type::FILE_SYSTEM) {
			return resolve_address_with_file_system(is_file, folder, extensions, id, provider.get_root_path());
		}
		else {
			ASSERT(false);
		}
		return resource_address();
	}

	resource_address resource_system::resolve_address_with_file_system(bool is_file, const char* folder, const char* extensions, const char* id, const std::string& root_path) {
		if (is_file) {
			for (auto& ext : split_string(extensions, ";")) {
				auto file_name = std::string(id) + ext;
				auto file_path = make_file_path(root_path, folder, file_name);
				if (_file_system.does_file_exist(file_path)) {
					return make_resource_address_with_file_system_provider(true, file_path);
				}
			}
		}
		else {
			ASSERT(is_string_empty(extensions));
			auto dir_path = make_file_path(root_path, folder, id);
			if (_file_system.does_directory_exist(dir_path)) {
				return make_resource_address_with_file_system_provider(false, dir_path);
			}
		}
		return resource_address();
	}

	resource_address resource_system::resolve_address_to_file_in_directory(
		const char* resource_type_name,
		const resource_address& dir_address,
		const char* extensions,
		const char* id) {

		ASSERT(dir_address.is_directory());
		ASSERT(dir_address.get_provider_type() == resource_provider_type::FILE_SYSTEM);

		if (is_string_empty(extensions)) {
			auto file_path = make_file_path(dir_address.get_path(), id);
			if (_file_system.does_file_exist(file_path)) {
				return make_resource_address_with_file_system_provider(true, file_path);
			}
		}

		for (auto& ext : split_string(extensions, ";")) {
			auto file_name = std::string(id) + ext;
			auto file_path = make_file_path(dir_address.get_path(), file_name);
			if (_file_system.does_file_exist(file_path)) {
				return make_resource_address_with_file_system_provider(true, file_path);
			}
		}

		ALERT("resource not found : {}{}\n\nresource_type: {}\ndirectory: {}", id, extensions, resource_type_name, dir_address.get_path());
		return resource_address();
	}


	stream* resource_system::open_stream_to_read(const resource_address& address) {
		return open_stream(address, file_mode::OPEN_READ);
	}

	stream* resource_system::open_stream_to_write(const resource_address& address) {
		return open_stream(address, file_mode::CREATE_WRITE);
	}

	stream* resource_system::open_stream(const resource_address& address, file_mode file_mode) {
		IF_VERIFY(!address.empty()) {
			if (address.get_provider_type() == resource_provider_type::FILE_SYSTEM) {
				auto fs = _file_system.open_file_stream(address.get_path(), file_mode);
				if (fs != nullptr) {
					_open_file_system_streams.push_back(fs);
					return fs;
				}
			}
			else {
				ASSERT(false);
			}
		}
		return nullptr;
	}

	void resource_system::close_stream(stream* stream) {
		if (try_find_and_erase(_open_file_system_streams, stream)) {
			auto fs = static_cast<file_stream*>(stream);
			_file_system.close_file_stream(fs);
		}
		else {
			ASSERT(false);
		}
	}

	bool resource_system::read_to_mapped_memory(resource_mapped_memory& mapped_memory, const resource_address& address) {
		IF_VERIFY(!address.empty()) {
			if (address.get_provider_type() == resource_provider_type::FILE_SYSTEM) {
				auto fs = _file_system.open_file_stream(address.get_path(), file_mode::OPEN_READ);
				if (fs != nullptr) {
					auto buffer_size = fs->get_size();
					char* buffer = mapped_memory.allocate_buffer(buffer_size, address.to_string().c_str());
					auto read_size = fs->read_bytes(buffer, buffer_size);
					ASSERT(read_size == buffer_size);
					_file_system.close_file_stream(fs);
					return true;
				}
			}
			else {
				ASSERT(false);
			}
		}
		return false;
	}

	bool resource_system::read_object_as_json(archivable& object, const resource_address& address) {
		auto stream = open_stream_to_read(address);
		if (stream != nullptr) {
			json_archive_reader reader(*stream);
			object.read_from_archive(reader);
			reader.attempt_raise_error_for_unused_values();
			close_stream(stream);
			return true;
		}
		return false;
	}

	void resource_system::write_object_as_json(const archivable& object, const resource_address& address) {
		auto stream = open_stream_to_write(address);
		if (stream != nullptr) {
			json_archive_writer writer(*stream);
			writer.begin_writing();
			object.write_to_archive(writer);
			writer.end_writing();
			close_stream(stream);
		}
	}

	bool resource_system::does_id_have_bad_chars(const char* id) const {
		int len = get_string_length(id);
		for (int i = 0; i < len; ++i) {
			char c = id[i];
			if (!::isdigit(c)) {
				if (::isalpha(c)) {
					if (::isupper(c)) {
						return true; //no uppercase chars allowed to prevent file system conflicts.
					}
				}
				else {
					if (c != '_') {
						return true; //only _ is allowed if not alphanumeric
					}
				}
			}
		}
		return false;
	}

	std::string resource_system::build_newline_seperated_providers_description() const {
		std::string d = "\n";
		for (auto& provider : _providers) {
			d += provider.get_description();
			d += "\n";
		}
		return d;
	}

	void resource_system::begin_watching_resource(file_change_handler* handler, const resource_address& address) {
		begin_watching_resource(handler, address, nullptr);
	}

	void resource_system::begin_watching_resource(file_change_handler* handler, const resource_address& address, void* data) {
		if (address.get_provider_type() == resource_provider_type::FILE_SYSTEM) {
			_file_change_watcher.begin_watching_file(handler, address.get_path(), data);
		}
	}

	void resource_system::end_watching_resource(file_change_handler* handler, void* data) {
		_file_change_watcher.end_watching_file(handler, data);
	}

	void resource_system::end_watching_resources(file_change_handler* handler) {
		_file_change_watcher.end_watching_files(handler);
	}

}