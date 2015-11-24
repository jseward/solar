#include "solar/networking/ip_address.h"

#include "win32_headers.h"

namespace solar {

	ip_address make_ip_address_any() {
		return ip_address(htonl(INADDR_ANY));
	}

	ip_address make_ip_address_local() {
		//http://tangentsoft.net/wskfaq/examples/ipaddr.html
		unsigned long addr = INADDR_NONE;

		char host_name[80];
		if (::gethostname(host_name, sizeof(host_name)) != SOCKET_ERROR) {
			hostent* he = ::gethostbyname(host_name);
			if (he != nullptr) {
				addr = *((unsigned long*)he->h_addr_list[0]);
			}
		}

		return ip_address(addr);
	}

}