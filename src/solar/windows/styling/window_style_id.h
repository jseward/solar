#pragma once

#include "solar/resources/resource_id.h"
#include "window_style.h"
#include "window_style_factory.h"

namespace solar {

	template<typename StyleT>
	class window_style_id : public resource_id<StyleT, window_style_factory> {
	private:
		const window& _owner_window;
		mutable StyleT _fallback_style;

	public:
		window_style_id(const window& owner_window) 
			: _owner_window(owner_window) {
		}

	private:
		virtual int get_current_resource_caching_context(const window_style_factory& factory) const override {
			return factory.get_window_style_caching_context();
		}

		virtual StyleT* get_uncached_resource(window_style_factory& factory, const std::string& id, const std::string& id_source_description) const override{
			return factory.get_window_style<StyleT>(_owner_window, _fallback_style, id, id_source_description);
		}
	};

}