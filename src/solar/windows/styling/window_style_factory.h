#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "solar/archiving/archivable.h"
#include "solar/utility/verify.h"
#include "solar/windows/window.h"
#include "solar/windows/window_helpers.h"
#include "solar/resources/resource_factory_caching_context.h"
#include "window_style.h"

namespace solar {

	class resource_system;

	class window_style_factory {
	private:
		class window_style_type_info : public archivable {
		private:
			std::unique_ptr<window_style> _style_def;
			std::unordered_map<std::string, std::unique_ptr<window_style>> _styles;

		public:
			explicit window_style_type_info(std::unique_ptr<window_style> style_def);
			template<typename StyleT> StyleT* get_window_style(StyleT& fallback_style, const std::string& id, const std::string& id_source_description);
			virtual void read_from_archive(archive_reader& reader) override;
			virtual void write_to_archive(archive_writer& writer) const override;
		};

	private:
		resource_system& _resource_system;

		resource_factory_caching_context _caching_context;
		std::unordered_map<std::string, std::unique_ptr<window_style_type_info>> _window_style_type_infos;

	public:
		window_style_factory(resource_system& resource_system);
		~window_style_factory();
		
		template<typename StyleT> void add_window_style_type();
		template<typename StyleT> void remove_window_style_type();

		template<typename StyleT> StyleT* get_window_style(const window& owner_window, StyleT& fallback_style, const std::string& id, const std::string& id_source_description);

		void load();

		const resource_factory_caching_context& get_caching_context() const;
		window_style* get_window_style(const char* window_style_typename, const std::string& id, const std::string& id_source_description);
	};

	template<typename StyleT> void window_style_factory::add_window_style_type() {
		ASSERT(_window_style_type_infos.find(StyleT::TYPE_NAME) == _window_style_type_infos.end());
		_window_style_type_infos[StyleT::TYPE_NAME] = std::make_unique<window_style_type_info>(std::make_unique<StyleT>());
		_caching_context.increment();

		window_style_id<StyleT>::set_factory(this);
	}

	template<typename StyleT> void window_style_factory::remove_window_style_type() {
		_window_style_type_infos.erase(StyleT::TYPE_NAME);
		_caching_context.increment();

		window_style_id<StyleT>::set_factory(nullptr);
	}

	template<typename StyleT> StyleT* window_style_factory::get_window_style(const window& owner_window, StyleT& fallback_style, const std::string& id, const std::string& id_source_description) {
		if (id.empty()) {
			ALERT("window_style_id has not been initialized.\n\ntype : {}\nowner_window : {}", StyleT::TYPE_NAME, make_window_hierarchy_id_string(owner_window));
		}
		else {
			auto type_info_iter = _window_style_type_infos.find(StyleT::TYPE_NAME);
			IF_VERIFY(type_info_iter != _window_style_type_infos.end()) {
				return type_info_iter->second->get_window_style(fallback_style, id, id_source_description);
			}
		}

		return &fallback_style;
	}

	template<typename StyleT> StyleT* window_style_factory::window_style_type_info::get_window_style(StyleT& fallback_style, const std::string& id, const std::string& id_source_description) {
		auto style_iter = _styles.find(id);
		if (style_iter == _styles.end()) {
			ALERT("window_style not found.\n\nid: '{}'\nid_source: {}", id, id_source_description);
			return &fallback_style;
		}
		return static_cast<StyleT*>(style_iter->second.get());
	}

}
