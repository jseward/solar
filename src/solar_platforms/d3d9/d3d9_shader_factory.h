#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/shaders/shader_factory.h"
#include "d3d9_shader.h"
#include "d3d9_headers.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_context;
	class resource_system;
	class resource_mapped_memory;

	class d3d9_shader_factory 
		: public shader_factory
		, public d3d9_device_event_handler {

	private:
		static const char* FALLBACK_SHADER_CODE;
		static const char* DEFAULT_STATES_SHADER_CODE;

	private:
		d3d9_context& _context;
		resource_system& _resource_system;
		bool _is_setup;
		resource_factory_caching_context _caching_context;
		bool _is_debug_shaders_enabled;
		std::unique_ptr<d3d9_shader> _default_states_shader;
		std::unordered_map<std::string, std::unique_ptr<d3d9_shader>> _shaders;
		std::vector<d3d9_shader*> _embeded_code_shaders;
		ID3DXEffectPool* _ID3DXEffectPool;
		std::unique_ptr<resource_mapped_memory> _resource_mapped_memory;

	public:
		d3d9_shader_factory(d3d9_context& context, resource_system& resource_system);
		virtual ~d3d9_shader_factory();

		void setup();
		void teardown();

		virtual void set_render_states_to_defaults() override;
		virtual shader* get_shader(const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_caching_context() const override;

		d3d9_shader* create_embeded_code_shader(const char* embedded_code);
		void release_embeded_code_shader(d3d9_shader* shader);

		ID3DXEffectPool* get_ID3DXEffectPool();
		DWORD get_d3dxcreateeffect_flags() const;
		resource_system& get_resource_system();
		resource_mapped_memory& get_resource_mapped_memory();

	private:
		void create_default_states_shader();
		void release_default_states_shader();
		void remove_all_shaders();

		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}