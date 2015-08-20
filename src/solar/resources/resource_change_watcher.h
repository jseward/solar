#pragma once

#include <vector>
#include "resource_provider.h"
#include "resource_change_handler.h"
#include "solar/time/real_time_timer.h"

namespace solar {

	class directory_change_watcher;

	class resource_change_watcher {
	private:
		class watching_resource_info {
		public:
			resource_change_handler* _handler;
			resource_address _address;

		public:
			watching_resource_info();
			watching_resource_info(resource_change_handler* handler, const resource_address& address);
		};

	private:
		directory_change_watcher& _directory_change_watcher;

		bool _is_setup;
		real_time_timer _try_handle_any_change_timer;
		std::vector<std::string> _watching_dir_paths;
		std::vector<watching_resource_info> _watching_resource_infos;

	public:
		resource_change_watcher(directory_change_watcher& directory_change_watcher);
		~resource_change_watcher();

		void setup(const std::vector<resource_provider>& providers);
		void teardown();
		
		void begin_watching_resource(resource_change_handler* handler, const resource_address& address);
		void end_watching_resource(resource_change_handler* handler);

		void try_handle_any_change();
	};


}