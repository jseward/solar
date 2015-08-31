#pragma once

#include <memory>
#include "solar/io/file_change_handler.h"
#include "solar/resources/resource_factory_caching_context.h"
#include "solar/resources/resource_address.h"
#include "text.h"
#include "text_map.h"

namespace solar {

	class resource_system;

	class text_factory : file_change_handler {
	private:
		resource_system& _resource_system;

		resource_factory_caching_context _caching_context;
		std::string _language;
		resource_address _language_address;
		text_map _text_map;
		std::unique_ptr<text> _empty_text;

	public:
		text_factory(resource_system& resource_system);

		void setup(const char* language);
		void teardown();

		const resource_factory_caching_context& get_caching_context() const;
		text* get_text(const std::string& id, const std::string& id_source_description);

	private:
		virtual void on_file_changed(const std::string& path, void* data) override;

	private:
		void load_from_address(const resource_address& address);
	};


}