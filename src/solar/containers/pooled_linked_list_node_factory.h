#pragma once

#include "solar/utility/memory_pool.h"
#include "linked_list.h"

namespace solar {

	template<typename T>
	class pooled_linked_list_node_factory {
	public:
		typedef linked_list_node<T> node_type;

	private:
		memory_pool _pool;

	public:
		pooled_linked_list_node_factory(const char* pool_id) 
			: _pool(pool_id) {
		}

		void allocate_memory(size_t max_node_count) {
			_pool.allocate_memory(sizeof(node_type), max_node_count);
		}

		void deallocate_memory() {
			_pool.deallocate_memory();
		}

		node_type* create_node() {
			return _pool.create_object<node_type>();
		}

		void release_node(node_type* node) {
			_pool.release_object(node);
		}
	};

}