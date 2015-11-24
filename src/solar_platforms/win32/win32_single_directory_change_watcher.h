#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <array>
#include <unordered_map>
#include "solar/io/directory_change.h"
#include "solar/threading/thread_safe_queue.h"
#include "win32_unique_handle.h"

namespace solar {

	class win32_single_directory_change_watcher {

	private:
		//need to stage all changes before they are considered valid changes so that we can
		//smartly combine duplicate changes or discard redundant changes.
		//http://blogs.msdn.com/b/oldnewthing/archive/2014/05/08/10523172.aspx
		class staged_change {
		public:
			DWORD _fni_action;
			float _last_change_time;

		public:
			staged_change() 
				: _fni_action(0)
				, _last_change_time(0.f) {
			}

			staged_change(DWORD fni_action, float last_change_time)
				: _fni_action(fni_action)
				, _last_change_time(last_change_time) {
			}
		};

	private:
		const std::string _dir_path;
		const bool _is_recursive;

		std::unique_ptr<std::thread> _reading_thread;
		win32_unique_handle _h_dir;
		win32_unique_handle _h_iocp;
		std::mutex _staged_changes_mutex;
		std::unordered_map<std::string, staged_change> _staged_changes;
		std::queue<directory_change> _changes;

	public:
		win32_single_directory_change_watcher(const std::string& dir_path, bool is_recursive);
		~win32_single_directory_change_watcher();

		const std::string& get_dir_path() const;
		void begin_watching();
		void end_watching();
		bool try_pop_directory_change(directory_change& change);

	private:
		void try_complete_staged_changes();
		void run_in_thread(HANDLE h_iocp, HANDLE h_dir, bool is_recursive);
		void handle_notifications_in_thread(const char* buffer, DWORD size);
		void handle_notification_in_thread(const FILE_NOTIFY_INFORMATION* fni);
	};

}
