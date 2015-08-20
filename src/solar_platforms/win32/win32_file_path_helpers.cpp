#include "solar/io/file_path_helpers.h"
#include "solar/utility/verify.h"
#include "solar/strings/string_marshal.h"

namespace solar {

	namespace win32_file_path_helpers_impl{
		class split_path_result {
		public:
			std::string _drive;
			std::string _dir;
			std::string _file_name;
			std::string _ext;
		};

		split_path_result split_path(const std::string& path) {
			wchar_t drive[_MAX_DRIVE];
			wchar_t dir[_MAX_DIR];
			wchar_t file_name[_MAX_FNAME];
			wchar_t ext[_MAX_EXT];
			VERIFY(::_wsplitpath_s(utf8_to_utf16(path).c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, file_name, _MAX_FNAME, ext, _MAX_EXT) == 0);
			
			split_path_result result;
			result._drive = utf16_to_utf8(drive);
			result._dir = utf16_to_utf8(dir);
			result._file_name = utf16_to_utf8(file_name);
			result._ext = utf16_to_utf8(ext);
			return result;
		}
	}

	const std::string FILE_PATH_SEPERATOR_CHAR = "\\";
	const std::wstring FILE_PATH_SEPERATOR_WCHAR = L"\\";

	std::string get_file_extension(const std::string& path) {
		return win32_file_path_helpers_impl::split_path(path)._ext;
	}

	std::string get_file_directory(const std::string& path) {
		return win32_file_path_helpers_impl::split_path(path)._dir;
	}

	std::string get_file_name_no_path(const std::string& path) {
		auto split_result = win32_file_path_helpers_impl::split_path(path);
		return split_result._file_name + split_result._ext;
	}

	std::string get_file_name_no_path_no_extension(const std::string& path) {
		auto split_result = win32_file_path_helpers_impl::split_path(path);
		return split_result._file_name;
	}

	std::string change_file_path_extension(const std::string& path, const std::string& new_extension) {
		auto split_result = win32_file_path_helpers_impl::split_path(path);
		return split_result._drive + split_result._dir + split_result._file_name + new_extension;
	}

}
