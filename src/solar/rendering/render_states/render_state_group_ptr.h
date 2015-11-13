#pragma once

#include <memory>
#include <functional>
#include "render_state_group.h"
#include "../render_device.h"

namespace solar {

	typedef std::unique_ptr<render_state_group, std::function<void(render_state_group*)>> render_state_group_ptr;

	inline render_state_group_ptr make_render_state_group_ptr(render_device& render_device, const render_state_group_def& def) {
		auto del = [&render_device](render_state_group* rsg) { render_device.release_render_state_group(rsg); };
		return render_state_group_ptr(render_device.create_render_state_group(def), del);
	}

}