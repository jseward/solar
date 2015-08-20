#pragma once

#include <memory>

namespace solar {

	class file_stream;

	//useful for reading an entire stream into memory so it can be passed to 3rd party api (ex. D3DX, FMOD)
	class resource_mapped_memory {
	private:
		std::unique_ptr<char[]> _fixed_buffer;
		unsigned int _fixed_buffer_size;
		std::unique_ptr<char[]> _dynamic_buffer;
		unsigned int _dynamic_buffer_size;
		bool _is_locked;
		unsigned int _needed_buffer_size;

	public:
		resource_mapped_memory();
		~resource_mapped_memory();

		void set_fixed_buffer_size_in_mb(float mb);

		void lock();
		void unlock();

		char* allocate_buffer(unsigned int size, const char* description);
		const char* get_buffer() const;
		unsigned int get_buffer_size() const;

	private:
		resource_mapped_memory(const resource_mapped_memory&) = delete;
		resource_mapped_memory& operator=(const resource_mapped_memory&) = delete;
	};

}