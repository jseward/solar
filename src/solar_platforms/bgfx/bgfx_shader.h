#pragma once

#include "solar/rendering/shaders/shader.h"
#include "solar/resources/resource_address.h"

namespace solar {

	class bgfx_shader : public shader {
	private:
		resource_address _resource_address;

	public:
		bgfx_shader(const resource_address& resource_address);
		virtual ~bgfx_shader();

		const resource_address& get_resource_address() const;

		virtual void start(const char* technique) override;
		virtual void stop() override;
		virtual void commit_param_changes() override;
		virtual void forget_param_changes() override;
		virtual bool set_bool(const char* name, bool value) override;
		virtual bool set_float(const char* name, float value) override;
		virtual bool set_float_array(const char* name, const float* values, unsigned int count) override;
		virtual bool set_mat44(const char* name, const mat44& value) override;
		virtual bool set_texture(const char* name, texture& texture) override;
		virtual bool set_platform_texture(const char* name, void* texture) override;
	};
	
}