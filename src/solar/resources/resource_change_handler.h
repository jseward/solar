#pragma once

#include "resource_address.h"

namespace solar {

	class resource_change_handler {
	public:
		virtual void on_resource_changed(const resource_address& address) = 0;
	};

}