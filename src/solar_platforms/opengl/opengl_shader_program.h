#pragma once

#include "solar/rendering/shaders/shader_program.h"
#include "solar/resources/resource_address.h"
#include "opengl_shader_program_def.h"

namespace solar {

	class resource_system;
	class opengl_shader_program_factory;

	class opengl_shader_program : public shader_program {
	private:
		opengl_shader_program_factory& _factory;
		const resource_address _resource_address;

	public:
		opengl_shader_program(opengl_shader_program_factory& factory, const resource_address& resource_address);
		opengl_shader_program(opengl_shader_program_factory& factory, const char* vs_embedded_code, const char* fs_embedded_code);
		virtual ~opengl_shader_program();

		const resource_address& get_resource_address() const;

		void load();
		void create_embedded();

		virtual void start() override;
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