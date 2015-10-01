#include "server.h"

#include <assert.h>
#include "global_state.h"
#include "set_current_thread_name.h"

namespace enetpp {

	server::server(thread_safe_trace_func trace_func) 
		: _trace_func(trace_func) {
	}

	server::~server() {
		//responsibility of owners to make sure stop_listening is always called. not calling stop_listening() in destructor due to
		//trace_func side effects.
		assert(_thread == nullptr);
		assert(_packet_queue.empty());
		assert(_event_queue.empty());
		assert(_event_queue_copy.empty());
	}

	bool server::is_listening() const {
		return (_thread != nullptr);
	}

	void server::start_listening(const server_listen_params& params) {
		assert(global_state::is_initialized());
		assert(!is_listening());
		assert(params._channel_count > 0);
		assert(params._listen_port != 0);

		_should_exit_thread = false;
		_thread = std::make_unique<std::thread>(&server::run_in_thread, this, params);
	}

	void server::stop_listening() {
		if (_thread != nullptr) {
			_should_exit_thread = true;
			_thread->join();
			_thread.release();
		}

		destroy_all_queued_packets();
		destroy_all_queued_events();
	}

	void server::send_packet_async(enet_uint8 channel, ENetPacket* packet, unsigned int peer_uid) {
		assert(is_listening());
		if (_thread != nullptr) {
			std::lock_guard<std::mutex> lock(_packet_queue_mutex);
			_packet_queue.emplace(channel, packet, peer_uid);
		}
	}

	void server::consume_events(std::function<void(const server_peer_event&)> event_handler) {
		if (!_event_queue.empty()) {

			{
				std::lock_guard<std::mutex> lock(_event_queue_mutex);
				assert(_event_queue_copy.empty());
				_event_queue_copy = _event_queue;
				_event_queue = {};
			}

			while (!_event_queue_copy.empty()) {
				auto& e = _event_queue_copy.front();
				event_handler(e);
				destroy_event_data(e);
				_event_queue_copy.pop();
			}
		}
	}

	void server::run_in_thread(const server_listen_params& params) {
		set_current_thread_name("enetpp::server");

		auto address = params.make_listen_address();
		ENetHost* host = enet_host_create(
			&address,
			params._max_client_count,
			params._channel_count,
			params._incoming_bandwidth,
			params._outgoing_bandwidth);
		if (host == nullptr) {
			trace("enet_host_create failed");
		}

		while (host != nullptr) {
			
			if (_should_exit_thread) {
				disconnect_all_peers_in_thread();
				enet_host_destroy(host);
				host = nullptr;
			}

			if (host != nullptr) {
				send_queued_packets_in_thread();
				capture_events_in_thread(params, host);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void server::disconnect_all_peers_in_thread() {
		for (auto iter : _peer_map) {
			enet_peer_disconnect_now(iter.second, 0);
			iter.second->data = nullptr;
		}
		_peer_map.clear();
	}

	void server::send_queued_packets_in_thread() {
		if (!_packet_queue.empty()) {
			std::lock_guard<std::mutex> lock(_packet_queue_mutex);
			while (!_packet_queue.empty()) {
				auto qp = _packet_queue.front();
				_packet_queue.pop();

				auto pi = _peer_map.find(qp._peer_uid);
				if (pi != _peer_map.end()) {

					//enet_peer_send fails if state not connected. was getting random asserts on peers disconnecting and going into ENET_PEER_STATE_ZOMBIE.
					if (pi->second->state == ENET_PEER_STATE_CONNECTED) {

						if (enet_peer_send(pi->second, qp._channel, qp._packet) != 0) {
							trace("enet_peer_send failed");
						}

						if (qp._packet->referenceCount == 0) {
							enet_packet_destroy(qp._packet);
						}
					}
				}
			}
		}
	}
				
	void server::capture_events_in_thread(const server_listen_params& params, ENetHost* host) {
		//http://lists.cubik.org/pipermail/enet-discuss/2013-September/002240.html
		enet_host_service(host, 0, 0);

		ENetEvent e;
		while (enet_host_check_events(host, &e) > 0) {
			switch (e.type) {
				case ENET_EVENT_TYPE_CONNECT: {
					handle_connect_event_in_thread(params, e);
					break;
				}

				case ENET_EVENT_TYPE_RECEIVE: {
					handle_receive_event_in_thread(e);
					break;
				}

				case ENET_EVENT_TYPE_DISCONNECT: {
					handle_disconnect_event_in_thread(e);
					break;
				}

				case ENET_EVENT_TYPE_NONE:
				default:
					assert(false);
					break;
			}
		}
	}

	void server::handle_connect_event_in_thread(const server_listen_params& params, const ENetEvent& e) {
		enet_uint32 enet_timeout = static_cast<enet_uint32>(params._peer_timeout.count());
		enet_peer_timeout(e.peer, 0, enet_timeout, enet_timeout);

		char peer_ip[256];
		enet_address_get_host_ip(&e.peer->address, peer_ip, 256);

		//!IMPORTANT! ENET_EVENT_TYPE_CONNECT is not queued as an event to be processed on the main thread because
		//the server_peer_interface* attached to the peer MUST be created immediately in this worker thread. Otherwise
		//there is a chance the first few packets are received on the worker thread when the peer is not initialized 
		//causing them to be discarded.

		auto context = params._create_peer_context_func(peer_ip);

		assert(e.peer->data == nullptr);
		e.peer->data = context;

		_peer_map[context->get_uid()] = e.peer;
	}

	void server::handle_receive_event_in_thread(const ENetEvent& e) {
		if (e.peer->data != nullptr) {
			std::lock_guard<std::mutex> lock(_event_queue_mutex);
			_event_queue.emplace(ENET_EVENT_TYPE_RECEIVE, e.channelID, e.packet, reinterpret_cast<server_peer_context*>(e.peer->data));
		}
	}

	void server::handle_disconnect_event_in_thread(const ENetEvent& e) {
		if (e.peer->data != nullptr) {
			auto peer_context = reinterpret_cast<server_peer_context*>(e.peer->data);
			auto iter = _peer_map.find(peer_context->get_uid());
			assert(iter != _peer_map.end());
			assert(iter->second == e.peer);
			e.peer->data = nullptr;
			_peer_map.erase(iter);

			std::lock_guard<std::mutex> lock(_event_queue_mutex);
			_event_queue.emplace(ENET_EVENT_TYPE_DISCONNECT, 0, nullptr, peer_context);
		}
	}

	void server::destroy_all_queued_packets() {
		std::lock_guard<std::mutex> lock(_packet_queue_mutex);
		while (!_packet_queue.empty()) {
			enet_packet_destroy(_packet_queue.front()._packet);
			_packet_queue.pop();
		}
	}

	void server::destroy_all_queued_events() {
		std::lock_guard<std::mutex> lock(_event_queue_mutex);
		while (!_event_queue.empty()) {
			destroy_event_data(_event_queue.front());
			_event_queue.pop();
		}
	}

	void server::destroy_event_data(server_peer_event& e) {
		if (e._event_type == ENET_EVENT_TYPE_RECEIVE) {
			enet_packet_destroy(e._packet);
		}
	}

	void server::trace(const std::string& s) {
		if (_trace_func != nullptr) {
			_trace_func(s);
		}
	}

}