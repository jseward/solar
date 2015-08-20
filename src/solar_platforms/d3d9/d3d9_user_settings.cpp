#include "d3d9_user_settings.h"

#include "solar/utility/trace.h"
#include "solar/strings/string_convert.h"
#include "d3d9_string_convert.h"

namespace solar {

	void d3d9_user_settings::trace_dump() const {
		TRACE("window_type : {0}", d3d9_window_type_details::get_string(get_window_type()));
		TRACE("window_top_left : {0}", get_window_top_left());
		TRACE("is_window_constrainted_to_desktop : {0}", bool_to_string(get_is_window_constrainted_to_desktop()));
		TRACE("is_cursor_clipped : {0}", bool_to_string(get_is_cursor_clipped()));
		TRACE("adapter_ordinal : {0}", get_adapter_ordinal());
		TRACE("adapter_format : {0}", D3DFORMAT_to_string(get_adapter_format()));
		TRACE("depth_stencil_format : {0}", D3DFORMAT_to_string(get_depth_stencil_format()));
		TRACE("backbuffer_format : {0}", D3DFORMAT_to_string(get_backbuffer_format()));
		TRACE("backbuffer_size : {0}", get_backbuffer_size());
		TRACE("fullscreen_refresh_rate : {0}", get_fullscreen_refresh_rate());
	}

}