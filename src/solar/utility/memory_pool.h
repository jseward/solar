#pragma once

#include "solar/utility/assert.h"

namespace solar {
	
	enum class memory_pool_can_exceed_max_object_count { NO, YES };

	class memory_pool {
	private:
		struct node {
			node* _next;
		};

	private:
		const char* _id;
		const memory_pool_can_exceed_max_object_count _can_exceed_max_object_count;

		size_t _max_object_size;
		size_t _max_object_count;
		size_t _created_object_count;
		size_t _highest_object_count;
		char* _buffer;
		node* _next_free_node;
		bool _is_max_object_count_exceeded_first_time;

	public:
		memory_pool(const char* id, memory_pool_can_exceed_max_object_count can_exceed);
		~memory_pool();
		
		void allocate_memory(size_t max_object_size, size_t max_object_count);
		void deallocate_memory();

		unsigned int get_created_object_count() const;

		template<typename T, typename...Args> T* create_object(Args&&... args);
		template<typename T> void release_object(T* obj);

	private:
		node* get_node_ptr_at(size_t at);
		bool is_object_ptr_in_buffer(const void* ptr);
		char* create_object_buffer(size_t object_size);
		void release_object_buffer(char* object_buffer);
		void on_max_object_count_exceeded();
	};

	template<typename T, typename...Args> T* memory_pool::create_object(Args&&... args) {
		char* object_buffer = create_object_buffer(sizeof(T));
		return new(object_buffer) T(args...);
	}

	template<typename T> void memory_pool::release_object(T* obj) {
		obj->~T();
		release_object_buffer(reinterpret_cast<char*>(obj));
	}

}
