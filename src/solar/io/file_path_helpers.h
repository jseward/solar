#pragma once

#include <string>

namespace solar {
		
	//PLATFORM DEPENDENT
	extern const std::string FILE_PATH_SEPERATOR_CHAR;

	bool is_file_path_extension(const std::string& extension);

	std::string get_file_extension(const std::string& file_path);
	std::string get_file_directory(const std::string& file_path);
	std::string get_file_name_no_path(const std::string& file_path);
	std::string get_file_name_no_path_no_extension(const std::string& file_path);
	std::string change_file_path_extension(const std::string& file_path, const std::string& new_extension);

	std::string make_file_path(const std::string& part1, const std::string& part2);
	std::string make_file_path(const std::string& part1, const std::string& part2, const std::string& part3);

}