#include "global_state.h"

#include <assert.h>
#include "solar_dependencies/enet/include/enet/enet.h"

namespace enetpp {

	bool global_state::_is_initialized = false;

	bool global_state::is_initialized() {
		return _is_initialized;
	}

	void global_state::initialize() {
		assert(!_is_initialized);
		enet_initialize();
		_is_initialized = true;
	}

	void global_state::deinitialize() {
		enet_deinitialize();
		_is_initialized = false;
	}

}