#pragma once

#include "d3d9_headers.h"
#include "d3d9_window_type.h"
#include "solar/utility/property.h"
#include "solar/math/point.h"
#include "solar/math/size.h"

namespace solar {
	
	struct d3d9_device_params {
		MAKE_PROPERTY(d3d9_device_params, d3d9_window_type, window_type, d3d9_window_type::RESIZABLE_WINDOW);
		MAKE_PROPERTY(d3d9_device_params, point, window_top_left, point());
		MAKE_PROPERTY(d3d9_device_params, bool, is_window_constrainted_to_desktop, true);
		MAKE_PROPERTY(d3d9_device_params, D3DDEVTYPE, device_type, D3DDEVTYPE_HAL);
 		MAKE_PROPERTY(d3d9_device_params, UINT, adapter_ordinal, D3DADAPTER_DEFAULT);
		MAKE_PROPERTY(d3d9_device_params, D3DFORMAT, adapter_format, D3DFMT_R8G8B8);
		MAKE_PROPERTY(d3d9_device_params, DWORD, behavior_flags, D3DCREATE_SOFTWARE_VERTEXPROCESSING);
		MAKE_PROPERTY(d3d9_device_params, D3DPRESENT_PARAMETERS, present_parameters, D3DPRESENT_PARAMETERS());

		size get_backbuffer_size() const;

		void trace_dump() const;
	};

}