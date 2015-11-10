#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/textures/texture_factory.h"

namespace solar {

	class resource_system;
	class bgfx_texture;

	class bgfx_texture_factory : public texture_factory {
	private:
		resource_system& _resource_system;
		bool _is_setup;
		resource_factory_caching_context _caching_context;
		std::unordered_map<std::string, std::unique_ptr<bgfx_texture>> _textures;

	public:
		bgfx_texture_factory(resource_system& resource_system);
		virtual ~bgfx_texture_factory();

		void setup();
		void teardown();

		virtual void add_texture_pool(const char* texture_pool_name, unsigned int max_size_in_bytes, bool should_create_mip_maps) override;
		virtual texture* get_texture(const char* texture_pool_name, const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_caching_context() const override;

		resource_system& get_resource_system();

	private:
		void remove_all_textures();
	};

}
