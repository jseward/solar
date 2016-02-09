#include "d3d9_texture_factory.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/math/unit_convert.h"
#include "solar/resources/resource_system.h"
#include "d3d9_context.h"
#include "d3d9_texture.h"

namespace solar {

	d3d9_texture_factory::d3d9_texture_factory(d3d9_context& context, resource_system& resource_system)
		: _context(context)
		, _resource_system(resource_system)
		, _is_setup(false) {

		_resource_mapped_memory = std::make_unique<resource_mapped_memory>();
		_resource_mapped_memory->set_fixed_buffer_size_in_mb(1.f);
	}

	d3d9_texture_factory::~d3d9_texture_factory() {
		ASSERT(!_is_setup);
		ASSERT(_textures.empty());
	}

	void d3d9_texture_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
		_context.add_device_event_handler(this);
	}

	void d3d9_texture_factory::teardown() {
		ASSERT(_textures.empty());
		_context.remove_device_event_handler(this);
		_is_setup = false;
	}

	texture* d3d9_texture_factory::create_texture(const resource_address& address, const texture_create_params& params) {
		ASSERT(!address.empty());

		auto iter = _textures.find(address);
		if (iter != _textures.end()) {
			if (iter->second->get_create_params() != params) {
				ALERT("d3d9_texture created with different create_params.\n\naddress: {}", address);
			}
			iter->second->add_ref();
			return iter->second.get();
		}

		d3d9_texture* new_texture = new d3d9_texture(*this, address, params);
		new_texture->add_ref();
		//NOTE: texture resources aren't actually created until requested.
		_textures[address] = std::unique_ptr<d3d9_texture>(new_texture);
		return new_texture;
	}

	void d3d9_texture_factory::release_texture(texture* base_texture) {
		auto texture = static_cast<d3d9_texture*>(base_texture);
		texture->sub_ref();
		if (texture->get_ref_count() == 0) {
			texture->on_device_lost(_context.get_device());
			texture->on_device_released(_context.get_device());
			_textures.erase(texture->get_address());
		}
	}

	void d3d9_texture_factory::on_device_created(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_created(device);
		}
	}

	void d3d9_texture_factory::on_device_released(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_released(device);
		}
	}

	void d3d9_texture_factory::on_device_reset(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_reset(device);
		}
	}

	void d3d9_texture_factory::on_device_lost(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_lost(device);
		}
	}

	d3d9_context& d3d9_texture_factory::get_d3d9_context() {
		return _context;
	}

	resource_system& d3d9_texture_factory::get_resource_system() {
		return _resource_system;
	}

	resource_mapped_memory& d3d9_texture_factory::get_resource_mapped_memory() {
		return *_resource_mapped_memory.get();
	}

}
