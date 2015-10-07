#pragma once

#include "ip_address.h"

namespace solar {

	class ip_end_point {
	private:
		ip_address _address;
		int _port;

	public:
		ip_end_point();
		ip_end_point(const ip_address& address, int port);

		const ip_address& get_address() const;
		int get_port() const;
	};

}