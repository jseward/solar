#pragma once

#include "solar_dependencies/enet/include/enet/enet.h"

namespace enetpp {

	class client_queued_packet {
	public:
		enet_uint8 _channel;
		ENetPacket* _packet;

	public:
		client_queued_packet()
			: _channel(0)
			, _packet(nullptr) {
		}

		client_queued_packet(enet_uint8 channel, ENetPacket* packet)
			: _channel(channel)
			, _packet(packet) {
		}
	};

}