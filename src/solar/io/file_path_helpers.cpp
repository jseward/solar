#include "file_path_helpers.h"

#include "solar/utility/assert.h"

namespace solar {

	bool is_file_path_extension(const std::string& extension) {
		return extension.length() > 0 && extension.front() == '.';
	}

	std::string make_file_path(const std::string& part1, const std::string& part2) {
		ASSERT(FILE_PATH_SEPERATOR_CHAR.length() == 1);

		std::string file_path;

		if (!part1.empty()) {
			file_path.append(part1);
			if (part1.back() != FILE_PATH_SEPERATOR_CHAR.front()) {
				file_path.append(FILE_PATH_SEPERATOR_CHAR);
			}
		}

		if (!part2.empty()) {
			if (part2.front() == FILE_PATH_SEPERATOR_CHAR.front()) {
				file_path.append(part2.begin() + 1, part2.end());
			}
			else {
				file_path.append(part2);
			}
		}

		return file_path;
	}

	std::string make_file_path(const std::string& part1, const std::string& part2, const std::string& part3) {
		return make_file_path(make_file_path(part1, part2), part3);
	}

}
