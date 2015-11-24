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

	public:
		friend size operator*(const size& lhs, float rhs);
		friend size operator-(const size& lhs, const size& rhs);
		friend std::ostream& operator<<(std::ostream& os, const size& s);
	};

}