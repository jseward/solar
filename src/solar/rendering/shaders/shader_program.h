#pragma once

namespace solar {
	
	class mat44;
	class texture;

	class shader_program {
	public:
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void commit_param_changes() = 0;
		virtual void forget_param_changes() = 0;
		virtual bool set_bool(const char* name, bool value) = 0;
		virtual bool set_float(const char* name, float value) = 0;
		virtual bool set_float_array(const char* name, const float* values, unsigned int count) = 0;
		virtual bool set_mat44(const char* name, const mat44& value) = 0;
		virtual bool set_texture(const char* name, texture& texture) = 0;
		virtual bool set_platform_texture(const char* name, void* texture) = 0;
	};

}