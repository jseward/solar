#include "d3d9_device_params.h"

#include "solar/utility/trace.h"
#include "solar/strings/string_convert.h"
#include "d3d9_string_convert.h"

namespace solar {

	size d3d9_device_params::get_backbuffer_size() const {
		return size(get_present_parameters().BackBufferWidth, get_present_parameters().BackBufferHeight);
	}

	void d3d9_device_params::trace_dump() const {
		TRACE("window_type : {}", d3d9_window_type_details::get_string(get_window_type()));
		TRACE("device_type : {}", D3DDEVTYPE_to_string(get_device_type()));
		TRACE("adapter_ordinal : {}", get_adapter_ordinal());
		TRACE("adapter_format : {}", D3DFORMAT_to_string(get_adapter_format()));
		TRACE("behavior_flags : {}", get_behavior_flags());
		TRACE("pp.BackBufferWidth : {}", get_present_parameters().BackBufferWidth);
		TRACE("pp.BackBufferHeight : {}", get_present_parameters().BackBufferHeight);
		TRACE("pp.BackBufferFormat : {}", D3DFORMAT_to_string(get_present_parameters().BackBufferFormat));
		TRACE("pp.DepthStencilFormat : {}", D3DFORMAT_to_string(get_present_parameters().AutoDepthStencilFormat));
		TRACE("pp.DepthStencilEnabled : {}", bool_to_string(get_present_parameters().EnableAutoDepthStencil == TRUE));
	}

}