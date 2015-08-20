#include "resource_mapped_memory.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/utility/unit_convert.h"

namespace solar {

	resource_mapped_memory::resource_mapped_memory() 
		: _fixed_buffer_size(0)
		, _dynamic_buffer_size(0)
		, _is_locked(false)
		, _needed_buffer_size(0) {
	}

	resource_mapped_memory::~resource_mapped_memory() {
		ASSERT(!_is_locked);
		ASSERT(_needed_buffer_size == 0);
	}

	void resource_mapped_memory::set_fixed_buffer_size_in_mb(float mb) {
		ASSERT(!_is_locked);
		_fixed_buffer_size = mb_to_bytes(mb);
		_fixed_buffer.reset(new char[_fixed_buffer_size]);
	}

	void resource_mapped_memory::lock() {
		ASSERT(!_is_locked);
		_is_locked = true;
	}

	void resource_mapped_memory::unlock() {
		ASSERT(_is_locked);
		_is_locked = false;
		_needed_buffer_size = 0;

		_dynamic_buffer.release();
		_dynamic_buffer_size = 0;
	}

	char* resource_mapped_memory::allocate_buffer(unsigned int size, const char* description) {
		ASSERT(_is_locked);
		ASSERT(_needed_buffer_size == 0);
		_needed_buffer_size = size;
		if (_needed_buffer_size <= _fixed_buffer_size) {
			return _fixed_buffer.get();
		}

		TRACE("resource_mapped_memory is allocating dynamic buffer : {} ({} mb)", description, bytes_to_mb(_needed_buffer_size));
		_dynamic_buffer_size = _needed_buffer_size;
		_dynamic_buffer.reset(new char[_dynamic_buffer_size]);
		return _dynamic_buffer.get();
	}

	const char* resource_mapped_memory::get_buffer() const {
		ASSERT(_is_locked);
		if (_needed_buffer_size <= _fixed_buffer_size) {
			return _fixed_buffer.get();
		}
		else {
			return _dynamic_buffer.get();
		}
	}

	unsigned int resource_mapped_memory::get_buffer_size() const {
		ASSERT(_is_locked);	
		return _needed_buffer_size;
	}

}