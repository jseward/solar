#pragma once

#include <string>

namespace solar {

	class text {
	private:
		std::wstring _string;

	public:
		text(const std::wstring& string);

		operator const wchar_t*() const;
	};


}