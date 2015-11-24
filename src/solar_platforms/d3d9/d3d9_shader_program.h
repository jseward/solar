#pragma once

#include <string>
#include <unordered_map>
#include "solar/rendering/shaders/shader_program.h"
#include "solar/resources/resource_address.h"
#include "d3d9_headers.h"

namespace solar {

	class d3d9_shader_program_factory;

	class d3d9_shader_program : public shader_program {

	private:
		d3d9_shader_program_factory& _factory;
		resource_address _resource_address;
		const char* _embeded_code;
		ID3DXEffect* _ID3DXEffect;
		D3DXEFFECT_DESC _desc;
		std::unordered_map<std::string, D3DXHANDLE> _technique_handles;
		std::unordered_map<std::string, D3DXHANDLE> _param_handles;
		bool _is_started;
		bool _is_within_pass;
		bool _has_changes_to_commit;

	public:
		d3d9_shader_program(d3d9_shader_program_factory& factory, const resource_address& resource_address);
		d3d9_shader_program(d3d9_shader_program_factory& factory, const char* embeded_code);
		virtual ~d3d9_shader_program();

		const resource_address& get_resource_address() const;
		const char* get_embeded_code() const;

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

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);

		void start_with_flags(DWORD flags);

	private:
		void map_technique_handles();
		void map_param_handles();
		D3DXHANDLE get_param_to_change(const char* name);
		void begin_pass(unsigned int pass);
		void end_pass();
	};
	
}