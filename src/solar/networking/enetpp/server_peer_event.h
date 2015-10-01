#pragma once

#include "solar_dependencies/enet/include/enet/enet.h"
#include "server_peer_context.h"

namespace enetpp {

	//server can't use ENetEvent as ENetPeer is not thread safe. Instead track data that is safe.
	class server_peer_event {
	public:
		ENetEventType _event_type;
		enet_uint8 _channel;
		ENetPacket* _packet;
		server_peer_context* _peer_context;
	
	public:
		server_peer_event()
			: _event_type(ENET_EVENT_TYPE_NONE)
			, _channel(0)
			, _packet(nullptr)
			, _peer_context(nullptr) {
		}

		server_peer_event(ENetEventType event_type, enet_uint8 channel, ENetPacket* packet, server_peer_context* peer_context)
			: _event_type(event_type)
			, _channel(channel)
			, _packet(packet)
			, _peer_context(peer_context) {
		}
	};

}