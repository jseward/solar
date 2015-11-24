#include "memory_pool.h"

#include "solar/utility/alert.h"
#include "solar/utility/trace.h"

namespace solar {

	memory_pool::memory_pool(const char* id, memory_pool_can_exceed_max_object_count can_exceed)
		: _id(id)
		, _can_exceed_max_object_count(can_exceed) 
		
		, _max_object_size(0)
		, _max_object_count(0)
		, _created_object_count(0) 
		, _highest_object_count(0)
		, _buffer(nullptr)
		, _next_free_node(nullptr)
		, _is_max_object_count_exceeded_first_time(true) {
	}

	memory_pool::~memory_pool() {
		deallocate_memory();
	}

	void memory_pool::allocate_memory(size_t max_object_size, size_t max_object_count) {
		ASSERT(max_object_size > sizeof(node));
		ASSERT(max_object_count > 0);

		TRACE("memory_pool - {} : allocate_memory : object_size={} * object_count={} -> bytes={}", _id, max_object_size, max_object_count, max_object_size * max_object_count);

		_max_object_size = max_object_size;
		_max_object_count = max_object_count;
		if (max_object_count) {
			ASSERT(_buffer == nullptr);
			_buffer = new char[max_object_size * max_object_count];
			_next_free_node = reinterpret_cast<node*>(&_buffer[0]);
			for (size_t i = 0; i < (max_object_count - 1); i++) {
				get_node_ptr_at(i)->_next = get_node_ptr_at(i + 1);
			}
			get_node_ptr_at(max_object_count - 1)->_next = nullptr;
		}
	}

	void memory_pool::deallocate_memory() {
		if (_created_object_count != 0) {
			ALERT("pool({}) has {} unreleased objects", _id, _created_object_count);
		}

		if (_buffer != nullptr) {
			delete[] _buffer;
			_buffer = nullptr;
			_max_object_size = 0;
			_max_object_count = 0;
			_next_free_node = nullptr;
		}
	}

	unsigned int memory_pool::get_created_object_count() const {
		return _created_object_count;
	}

	memory_pool::node* memory_pool::get_node_ptr_at(size_t i) {
		ASSERT(i < _max_object_count);
		return reinterpret_cast<node*>(_buffer + (i * _max_object_size));
	}

	bool memory_pool::is_object_ptr_in_buffer(const void* ptr) {
		return (ptr >= _buffer) && (ptr < (_buffer + (_max_object_size * _max_object_count)));
	}

	char* memory_pool::create_object_buffer(size_t object_size) {
		ASSERT(object_size <= _max_object_size);
		ASSERT(_buffer != nullptr);

		char* object_buffer = nullptr;
		if (_created_object_count < _max_object_count) {
			object_buffer = reinterpret_cast<char*>(_next_free_node);
			_next_free_node = _next_free_node->_next;
		}
		else {
			ASSERT(_can_exceed_max_object_count == memory_pool_can_exceed_max_object_count::YES);
			on_max_object_count_exceeded();
			object_buffer = new char[object_size];
			ASSERT(!is_object_ptr_in_buffer(object_buffer));
		}

		_created_object_count++;
		_highest_object_count = std::max(_highest_object_count, _created_object_count);

		return object_buffer;
	}

	void memory_pool::release_object_buffer(char* object_buffer) {
		if (is_object_ptr_in_buffer(object_buffer)) {
			node* n = reinterpret_cast<node*>(object_buffer);
			n->_next = _next_free_node;
			_next_free_node = n;
		}
		else {
			delete[] object_buffer;
		}

		ASSERT(_created_object_count > 0);
		_created_object_count--;
	}

	void memory_pool::on_max_object_count_exceeded() {
		if (_is_max_object_count_exceeded_first_time) {
			_is_max_object_count_exceeded_first_time = false;
			TRACE("WARNING: pool({}) has run out of allocated memory.  new/delete is being used as a fallback...", _id);
		}
	}

}