#pragma once

#include <string>

namespace solar {
	
	class ip_address {
	private:
		unsigned long _address;

	public:
		ip_address();
		ip_address(unsigned long address);

		std::string to_string() const;

	public:
		//platform specific functions
		friend ip_address make_ip_address_any();
		friend ip_address make_ip_address_local();
	};

}
