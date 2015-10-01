#pragma once

#include "solar_dependencies/enet/include/enet/enet.h"

namespace enetpp {

	class server_queued_packet {
	public:
		enet_uint8 _channel;
		ENetPacket* _packet;
		unsigned int _peer_uid;

	public:
		server_queued_packet()
			: _channel(0)
			, _packet(nullptr) 
			, _peer_uid(0) {
		}

		server_queued_packet(enet_uint8 channel, ENetPacket* packet, unsigned int peer_uid)
			: _channel(channel)
			, _packet(packet) 
			, _peer_uid(peer_uid) {
		}
	};

}