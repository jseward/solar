#pragma once

#include <vector>
#include "solar/io/stream.h"
#include "resource_address.h"
#include "resource_provider.h"
#include "resource_mapped_memory.h"
#include "resource_system_params.h"

namespace solar {

	class archivable;
	class file_system;
	class file_change_watcher;
	class file_change_handler;
	enum class file_mode;

	class resource_system {

	private:
		file_system& _file_system;
		file_change_watcher& _file_change_watcher;
		std::vector<resource_provider> _providers;
		std::vector<stream*> _open_file_system_streams;

	public:
		resource_system(file_system& file_system, file_change_watcher& file_change_watcher);
		~resource_system();

		void setup(const resource_system_params& params);
		void teardown();

		std::vector<std::string> get_all_file_system_provider_dir_paths() const;

		resource_address resolve_address_to_file(
			const char* resource_type_name, 
			const char* folder, 
			const char* extensions, 
			const char* id, 
			const char* id_source_description);

		resource_address resolve_address_to_directory(
			const char* resource_type_name,
			const char* folder,
			const char* id,
			const char* id_source_description);

		stream* open_stream_to_read(const resource_address& address);
		stream* open_stream_to_write(const resource_address& address);
		void close_stream(stream* stream);

		bool read_to_mapped_memory(resource_mapped_memory& mapped_memory, const resource_address& address);

		bool read_object_as_json(archivable& object, const resource_address& address);
		void write_object_as_json(const archivable& object, const resource_address& address);

		void begin_watching_resource(file_change_handler* handler, const resource_address& address);
		void begin_watching_resource(file_change_handler* handler, const resource_address& address, void* data);
		void end_watching_resource(file_change_handler* handler, void* data);
		void end_watching_resources(file_change_handler* handler);

	private:
		resource_address resolve_address(bool is_file, const char* resource_type_name, const char* folder, const char* extensions, const char* id, const char* id_source_description);
		resource_address resolve_address_with_provider(bool is_file, const char* folder, const char* extensions, const char* id, const resource_provider& provider);
		resource_address resolve_address_with_file_system(bool is_file, const char* folder, const char* extensions, const char* id, const std::string& root_path);

		bool does_id_have_bad_chars(const char* id) const;

		std::string build_newline_seperated_providers_description() const;

		stream* open_stream(const resource_address& address, file_mode file_mode);
	};

}