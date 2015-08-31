#pragma once

#include <string>
#include <memory>
#include <vector>
#include "solar/time/date_time.h"
#include "file_stream.h"
#include "file_mode.h"
#include "find_file_names_options.h"

namespace solar {

	class file_system {
	public:
		virtual file_stream* open_file_stream(const std::string& path, file_mode mode) = 0;
		virtual void close_file_stream(file_stream* stream) = 0;
		virtual void delete_file(const std::string& path) = 0;
		virtual bool copy_file(const std::string& src_path, const std::string& dst_path) = 0;
		virtual void set_is_file_read_only(const std::string& path, bool is_read_only) = 0;
		virtual bool does_file_exist(const std::string& file_path) const = 0;
		virtual bool does_directory_exist(const std::string& dir_path) const = 0;
		virtual void create_directory(const std::string& dir_path) = 0;
		virtual std::string make_user_file_dir_path(const std::string& folder) = 0;
		virtual std::string make_user_file_path(const std::string& folder, const std::string& file_name) = 0;
		virtual std::string resolve_file_path(const std::string relative_path) = 0;
		virtual std::vector<std::string> find_file_names(const std::string& dir_path, const find_file_names_options& options) = 0;
		virtual bool is_directory(const std::string& path) = 0;
		virtual bool try_get_last_write_time(date_time& last_write_time, const std::string& path) = 0;
	};

}
