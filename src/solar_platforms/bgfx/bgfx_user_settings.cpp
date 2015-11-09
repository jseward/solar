#include "bgfx_user_settings.h"

#include "solar/utility/trace.h"

namespace solar {

	void bgfx_user_settings::trace_dump() const {
		TRACE("window_type : {0}", to_string(get_window_type()));
		if (get_window_type() == bgfx_window_type::RESIZABLE_WINDOW) {
			TRACE("window_client_size : {0}", get_window_client_size());
			TRACE("window_top_left : {0}", get_window_top_left());
			TRACE("is_window_constrainted_to_desktop : {0}", get_is_window_constrainted_to_desktop());
		}
	}

	bgfx::RendererType::Enum bgfx_user_settings::get_RendererType() const {
		return bgfx::RendererType::Count;//todo
	}

}
