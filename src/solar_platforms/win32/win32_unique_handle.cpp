#include "win32_unique_handle.h"
#include "solar/utility/assert.h"

namespace solar {

	win32_unique_handle::win32_unique_handle()
		: _handle(NULL) {
	}

	win32_unique_handle::~win32_unique_handle() {
		close();
	}

	win32_unique_handle& win32_unique_handle::operator=(HANDLE h) {
		ASSERT(_handle == NULL);
		_handle = h;
		return *this;
	}

	void win32_unique_handle::close() {
		if (_handle != NULL) {
			::CloseHandle(_handle);
			_handle = NULL;
		}
	}

	HANDLE win32_unique_handle::get() {
		return _handle;
	}

}