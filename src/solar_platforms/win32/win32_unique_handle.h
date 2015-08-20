#pragma once

#include "win32_headers.h"

namespace solar {

	//like std::unique_ptr but for HANDLE. Will auto close on destruction.

	class win32_unique_handle {
	private:
		HANDLE _handle;

	public:
		win32_unique_handle();
		win32_unique_handle(const win32_unique_handle&) = delete;
		win32_unique_handle& operator=(const win32_unique_handle&) = delete;
		~win32_unique_handle();
		win32_unique_handle& operator=(HANDLE h);
		void close();
		HANDLE get();
	};

}
