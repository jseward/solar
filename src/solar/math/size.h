#pragma once

#include <ostream>

namespace solar {

	class archive_reader;
	class archive_writer;

	class size {
	public:
		int _width;
		int _height;

	public:
		size();
		size(int w, int h);

		bool operator==(const size& rhs) const;
		bool operator!=(const size& rhs) const;
		size operator*(float rhs) const;

	public:
		friend std::ostream& operator<<(std::ostream& os, const size& s);
	};

}