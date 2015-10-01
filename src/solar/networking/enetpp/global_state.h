#pragma once

namespace enetpp {

	//ugh

	class global_state {
	private:
		static bool _is_initialized;

	public:
		static bool is_initialized();
		static void initialize();
		static void deinitialize();		
	};

}