#pragma once

#include <memory>
#include <vector>
#include "solar/io/directory_change_watcher.h"
#include "solar/threading/thread_safe_queue.h"
#include "win32_unique_handle.h"
#include "win32_single_directory_change_watcher.h"

namespace solar {

	class win32_directory_change_watcher : public directory_change_watcher {
	private:
		using watchers_type = std::vector<std::unique_ptr<win32_single_directory_change_watcher>>;

	private:
		watchers_type _watchers;

	public:
		win32_directory_change_watcher();
		virtual ~win32_directory_change_watcher();

		virtual void begin_watching_directory(const std::string& dir_path, bool is_recursive) override;
		virtual void end_watching_directory(const std::string& dir_path) override;
		virtual bool try_pop_directory_change(directory_change& change) override;
	};

}
