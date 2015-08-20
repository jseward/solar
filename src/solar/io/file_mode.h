#pragma once

namespace solar {

	enum class file_mode {
		OPEN_READ,
		CREATE_WRITE,
		APPEND_WRITE
	};

	struct file_mode_details {
		static const char* get_string(file_mode e);
	};

}