#pragma once

#include <memory>
#include "text.h"
#include "text_map.h"
#include "solar/resources/resource_change_handler.h"

namespace solar {

	class resource_system;

	class text_factory : resource_change_handler {
	private:
		resource_system& _resource_system;

		int _caching_context;
		std::string _language;
		resource_address _language_address;
		text_map _text_map;
		std::unique_ptr<text> _empty_text;

	public:
		text_factory(resource_system& resource_system);

		void setup(const char* language);
		void teardown();

		int get_text_caching_context() const;
		text* get_text(const std::string& id, const std::string& id_source_description);

	private:
		virtual void on_resource_changed(const resource_address& address) override;

	private:
		void load_from_address(const resource_address& address);
	};


}