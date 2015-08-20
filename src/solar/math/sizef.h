#pragma once

#include <ostream>

namespace solar {

	class sizef {
	private:
		float _width;
		float _height;

	public:
		sizef();
		sizef(float w, float h);

		float get_width() const;
		void set_width(float x);

		float get_height() const;
		void set_height(float y);

	public:
		friend std::ostream& operator<<(std::ostream& os, const sizef& s);
	};

}