#pragma once

#include "solar_dependencies/enet/include/enet/enet.h"
#include <chrono>
#include <functional>
#include "server_peer_context.h"

namespace enetpp {

	class server_listen_params {
	public:
		using create_peer_context_func = std::function<server_peer_context*(const char*)>;

	public:
		size_t _max_client_count;
		size_t _channel_count;
		enet_uint32 _incoming_bandwidth;
		enet_uint32 _outgoing_bandwidth;
		enet_uint16 _listen_port;
		std::chrono::milliseconds _peer_timeout;
		create_peer_context_func _create_peer_context_func;

	public:
		server_listen_params()
			: _max_client_count(0)
			, _channel_count(0)
			, _incoming_bandwidth(0)
			, _outgoing_bandwidth(0) 
			, _peer_timeout(0) {
		}

		server_listen_params& set_listen_port(enet_uint16 port) {
			_listen_port = port;
			return *this;
		}

		server_listen_params& set_channel_count(size_t channel_count) {
			_channel_count = channel_count;
			return *this;
		}

		server_listen_params& set_incoming_bandwidth(enet_uint32 bandwidth) {
			_incoming_bandwidth = bandwidth;
			return *this;
		}

		server_listen_params& set_outgoing_bandwidth(enet_uint32 bandwidth) {
			_outgoing_bandwidth = bandwidth;
			return *this;
		}

		server_listen_params& set_peer_timeout(std::chrono::milliseconds timeout) {
			_peer_timeout = timeout;
			return *this;
		}

		server_listen_params& set_create_peer_context_func(create_peer_context_func func) {
			_create_peer_context_func = func;
			return *this;
		}		

		ENetAddress make_listen_address() const {
			ENetAddress address;
			address.host = ENET_HOST_ANY;
			address.port = _listen_port;
			return address;
		}
	};

}