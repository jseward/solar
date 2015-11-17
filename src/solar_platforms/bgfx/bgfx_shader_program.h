#pragma once

#include <bgfx/bgfx.h>
#include "solar/rendering/shaders/shader_program.h"
#include "solar/resources/resource_address.h"
#include "bgfx_shader_program_def.h"
#include "bgfx_shader_type.h"
#include "bgfx_shader_embedded_code.h"

namespace solar {

	class resource_system;
	class bgfx_shader_program_factory;

	class bgfx_shader_program : public shader_program {
	private:
		bgfx_shader_program_factory& _factory;
		const resource_address _resource_address;
		bgfx_shader_embedded_code _vs_embedded_code;
		bgfx_shader_embedded_code _fs_embedded_code;

		bgfx_shader_program_def _program_def;
		bgfx::ProgramHandle _program_handle;

	public:
		bgfx_shader_program(bgfx_shader_program_factory& factory, const resource_address& resource_address);
		bgfx_shader_program(bgfx_shader_program_factory& factory, bgfx_shader_embedded_code vs_embedded_code, bgfx_shader_embedded_code fs_embedded_code);
		virtual ~bgfx_shader_program();

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

	private:
		void destroy_program_handle();

	private:
		static bgfx::ShaderHandle load_shader(resource_system& resource_system, const resource_address& def_resource_address, bgfx_shader_type shader_type, const std::string& name);
		static std::string get_shader_extension(bgfx_shader_type shader_type);
	};
	
}