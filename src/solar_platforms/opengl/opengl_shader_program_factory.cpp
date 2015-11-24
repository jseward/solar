#include "opengl_shader_program_factory.h"

#include "solar/containers/container_helpers.h"
#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"

namespace solar {

	opengl_shader_program_factory::opengl_shader_program_factory(opengl_context& context, resource_system& resource_system)
		: _context(context)
		, _resource_system(resource_system)
		, _is_setup(false) {
	}

	opengl_shader_program_factory::~opengl_shader_program_factory() {
		ASSERT(!_is_setup);
		ASSERT(_shader_programs.empty());
		ASSERT(_embeded_code_shader_programs.empty());
	}

	void opengl_shader_program_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void opengl_shader_program_factory::teardown() {
		remove_all_shaders();
		_is_setup = false;
	}

	void opengl_shader_program_factory::remove_all_shaders() {
		_resource_system.end_watching_resources(this);
		_shader_programs.clear();
		_shader_program_caching_context.increment();
	}

	shader_program* opengl_shader_program_factory::get_shader_program(const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _shader_programs.find(id);
		if (iter != _shader_programs.end()) {
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address("shader_program", "shaders", ".shader_program", id.c_str(), id_source_description.c_str());
		opengl_shader_program* new_program = new opengl_shader_program(*this, address);
		if (!address.empty()) {
			new_program->load();
			_resource_system.begin_watching_resource(this, address, new_program);
		}
		_shader_programs[id] = std::unique_ptr<opengl_shader_program>(new_program);
		return new_program;
	}

	const resource_factory_caching_context& opengl_shader_program_factory::get_shader_program_caching_context() const {
		return _shader_program_caching_context;
	}

	opengl_context& opengl_shader_program_factory::get_opengl_context() {
		return _context;
	}

	resource_system& opengl_shader_program_factory::get_resource_system() {
		return _resource_system;
	}

	opengl_shader_program* opengl_shader_program_factory::create_embeded_code_shader_program(const char* vs_embedded_code, const char* fs_embedded_code) {
		auto shader_program = new opengl_shader_program(*this, vs_embedded_code, fs_embedded_code);
		shader_program->create_embedded();
		_embeded_code_shader_programs.push_back(shader_program);
		return shader_program;
	}

	void opengl_shader_program_factory::release_embeded_code_shader_program(opengl_shader_program* program) {
		if (program != nullptr) {
			find_and_erase(_embeded_code_shader_programs, program);
			delete program;
		}
	}

	void opengl_shader_program_factory::on_file_changed(const std::string& path, void* data) {
		opengl_shader_program* program = reinterpret_cast<opengl_shader_program*>(data);
		ASSERT(program->get_resource_address().get_file_path() == path);
		program->load();
	}

}