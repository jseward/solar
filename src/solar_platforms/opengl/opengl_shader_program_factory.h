#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/shaders/shader_program_factory.h"
#include "solar/io/file_change_handler.h"
#include "opengl_shader_program.h"

namespace solar {

	class opengl_context;
	class resource_system;
	class resource_mapped_memory;

	class opengl_shader_program_factory 
		: public shader_program_factory
		, public file_change_handler {

	private:
		opengl_context& _context;
		resource_system& _resource_system;
		bool _is_setup;
		resource_factory_caching_context _shader_program_caching_context;
		std::unordered_map<std::string, std::unique_ptr<opengl_shader_program>> _shader_programs;
		std::vector<opengl_shader_program*> _embedded_code_shader_programs;

	public:
		opengl_shader_program_factory(opengl_context& context, resource_system& resource_system);
		virtual ~opengl_shader_program_factory();

		void setup();
		void teardown();

		virtual shader_program* get_shader_program(const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_shader_program_caching_context() const override;

		opengl_shader_program* create_embedded_code_shader_program(const char* vs_embedded_code, const char* fs_embedded_code);
		void release_embedded_code_shader_program(opengl_shader_program* program);

		opengl_context& get_opengl_context();
		resource_system& get_resource_system();

	private:
		void remove_all_shaders();
		
		virtual void on_file_changed(const std::string& path, void* data) override;
	};

}