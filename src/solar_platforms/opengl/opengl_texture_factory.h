#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/textures/texture_factory.h"

namespace solar {

	class resource_system;
	class opengl_texture;

	class opengl_texture_factory : public texture_factory {
	private:
		resource_system& _resource_system;
		bool _is_setup;
		std::unordered_map<std::string, std::unique_ptr<opengl_texture>> _textures;

	public:
		opengl_texture_factory(resource_system& resource_system);
		virtual ~opengl_texture_factory();

		void setup();
		void teardown();

		virtual texture* create_texture(const resource_address& address, const texture_create_params& params) override;
		virtual void release_texture(texture* texture) override;

		resource_system& get_resource_system();
	};

}
