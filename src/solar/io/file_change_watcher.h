#pragma once

#include <vector>
#include "file_change_handler.h"
#include "solar/time/real_time_timer.h"

namespace solar {

	class directory_change_watcher;

	class file_change_watcher {
	private:
		class watched_file_info {
		public:
			file_change_handler* _handler;
			std::string _path;

		public:
			watched_file_info();
			watched_file_info(file_change_handler* handler, const std::string& path);
		};

	private:
		directory_change_watcher& _directory_change_watcher;

		bool _is_setup;
		real_time_timer _try_handle_any_change_timer;
		std::vector<std::string> _watched_dir_paths;
		std::vector<watched_file_info> _watched_file_infos;

	public:
		file_change_watcher(directory_change_watcher& directory_change_watcher);
		file_change_watcher(const file_change_watcher&) = delete;
		file_change_watcher& operator=(const file_change_watcher&) = delete;
		~file_change_watcher();

		void setup(const std::vector<std::string>& dir_paths);
		void teardown();
		
		void begin_watching_file(file_change_handler* handler, const std::string& path);
		void end_watching_file(file_change_handler* handler);

		void try_handle_any_change();
	};


}