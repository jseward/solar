#pragma once

#include <vector>
#include "solar/utility/property.h"
#include "solar/io/file_system.h"
#include "win32_headers.h"
#include "win32_file_stream.h"
#include "win32_user_file_location.h"

namespace solar {

	struct win32_file_system_setup_params {
		MAKE_PROPERTY(win32_file_system_setup_params, win32_user_file_location, user_file_location, win32_user_file_location::USER_APPDATA);
		MAKE_PROPERTY(win32_file_system_setup_params, std::string, user_root_path_company_name, "");
		MAKE_PROPERTY(win32_file_system_setup_params, std::string, user_root_path_app_name, "");
	};

	class win32_file_system : public file_system {
	private:
		std::string _user_root_path;
		std::vector<win32_file_stream*> _open_file_streams;

	public:
		win32_file_system();
		virtual ~win32_file_system();

		void setup(const win32_file_system_setup_params& params);

		virtual file_stream* open_file_stream(const std::string& path, file_mode mode) override;
		virtual void close_file_stream(file_stream* stream);
		virtual void delete_file(const std::string& path) override;
		virtual bool copy_file(const std::string& src_path, const std::string& dst_path) override;
		virtual void set_is_file_read_only(const std::string& path, bool is_read_only) override;
		virtual bool does_file_exist(const std::string& file_path) const override;
		virtual bool does_directory_exist(const std::string& dir_path) const override;
		virtual void create_directory(const std::string& dir_path) override;
		virtual std::string make_user_file_path(const std::string& folder, const std::string& file_name) override;
		virtual std::string resolve_file_path(const std::string relative_path) override;
		virtual std::vector<std::string> find_file_names(const std::string& dir_path, const find_file_names_options& options) override;
		virtual bool is_directory(const std::string& path) override;
		virtual bool try_get_last_write_time(date_time& last_write_time, const std::string& path) override;

	private:
		void setup_user_root_path(win32_user_file_location location, const std::string& company_folder_name, const std::string& app_folder_name);
		void create_directories_if_needed(const std::string& path);
		bool is_find_data_hidden(const WIN32_FIND_DATAW& find_data) const;
		bool is_find_data_directory(const WIN32_FIND_DATAW& find_data) const;
		bool check_file_name_with_find_file_names_options(const std::string& file_name, const find_file_names_options& options) const;
		date_time convert_FILETIME_to_date_time(const FILETIME& ft) const;
	};

}