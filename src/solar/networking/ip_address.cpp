#include "ip_address.h"

namespace solar {

	ip_address::ip_address() 
		: _address(0) {
	}

	ip_address::ip_address(unsigned long address) 
		: _address(address) {
	}

	std::string ip_address::to_string() const {
		struct address_struct {
			union {
				struct { uint8_t s_b1, s_b2, s_b3, s_b4; } S_un_b;
				struct { uint16_t s_w1, s_w2; } S_un_w;
				uint32_t S_addr;
			} S_un;
		};

		const address_struct* s = reinterpret_cast<const address_struct*>(&_address);
		return
			std::to_string(s->S_un.S_un_b.s_b1) + "." +
			std::to_string(s->S_un.S_un_b.s_b2) + "." +
			std::to_string(s->S_un.S_un_b.s_b3) + "." +
			std::to_string(s->S_un.S_un_b.s_b4);
	}

}