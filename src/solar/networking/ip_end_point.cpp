#include "ip_end_point.h"

#include "solar/strings/string_build.h"

namespace solar {

	ip_end_point::ip_end_point() 
		: _address()
		, _port(0) {
	}

	ip_end_point::ip_end_point(const ip_address& address, int port) 
		: _address(address)
		, _port(port) {
	}

	const ip_address& ip_end_point::get_address() const {
		return _address;
	}

	int ip_end_point::get_port() const {
		return _port;
	}

	std::string ip_end_point::to_string() const {
		return build_string("{}:{}", _address.to_string(), _port);
	}

}