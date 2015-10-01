#include "client.h"

#include <assert.h>
#include <chrono>
#include "set_current_thread_name.h"
#include "global_state.h"

namespace enetpp {

	client::client(thread_safe_trace_func trace_func)
		: _trace_func(trace_func)
		, _should_exit_thread(false) {
	}

	client::~client() {
		//responsibility of owners to make sure disconnect is always called. not calling disconnect() in destructor due to
		//trace_func side effects.
		assert(_thread == nullptr);
		assert(_packet_queue.empty());
		assert(_event_queue.empty());
		assert(_event_queue_copy.empty());
	}

	bool client::is_connecting_or_connected() const {
		return _thread != nullptr;
	}

	void client::connect(const client_connect_params& params) {
		assert(global_state::is_initialized());
		assert(!is_connecting_or_connected());
		assert(params._channel_count > 0);
		assert(params._server_port != 0);
		assert(!params._server_host_name.empty());

		_should_exit_thread = false;
		_thread = std::make_unique<std::thread>(&client::run_in_thread, this, params);
	}

	void client::disconnect() {
		if (_thread != nullptr) {
			_should_exit_thread = true;
			_thread->join();
			_thread.release();
		}

		destroy_all_queued_packets();
		destroy_all_queued_events();
	}

	void client::destroy_all_queued_packets() {
		std::lock_guard<std::mutex> lock(_packet_queue_mutex);
		while (!_packet_queue.empty()) {
			enet_packet_destroy(_packet_queue.front()._packet);
			_packet_queue.pop();
		}
	}

	void client::destroy_all_queued_events() {
		std::lock_guard<std::mutex> lock(_event_queue_mutex);
		while (!_event_queue.empty()) {
			destroy_event_data(_event_queue.front());
			_event_queue.pop();
		}
	}

	void client::send_packet_async(enet_uint8 channel, ENetPacket* packet) {
		assert(is_connecting_or_connected());
		if (_thread != nullptr) {
			std::lock_guard<std::mutex> lock(_packet_queue_mutex);
			_packet_queue.emplace(channel, packet);
		}
	}

	void client::consume_events(std::function<void(const ENetEvent&)> event_handler) {
		if (!_event_queue.empty()) {
			
			//!IMPORTANT! neet to copy the events for consumption to prevent deadlocks!
			//ex.
			//- event = JoinGameFailed packet received
			//- causes event_handler to call client::disconnect
			//- client::disconnect deadlocks as the thread needs a critical section on events to exit
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

	void client::destroy_event_data(ENetEvent& e) {
		if (e.type == ENET_EVENT_TYPE_RECEIVE) {
			enet_packet_destroy(e.packet);
		}
	}

	void client::run_in_thread(const client_connect_params& params) {
		set_current_thread_name("enetpp::client");

		ENetHost* host = enet_host_create(nullptr, 1, params._channel_count, params._incoming_bandwidth, params._outgoing_bandwidth);
		if (host == nullptr) {
			trace("enet_host_create failed");
			return;
		}

		auto address = params.make_server_address();
		ENetPeer* peer = enet_host_connect(host, &address, params._channel_count, 0);
		if (peer == nullptr) {
			trace("enet_host_connect failed");
			enet_host_destroy(host);
			return;
		}

		enet_uint32 enet_timeout = static_cast<enet_uint32>(params._timeout.count());
		enet_peer_timeout(peer, 0, enet_timeout, enet_timeout);

		bool is_disconnecting = false;
		enet_uint32 disconnect_start_time = 0;

		while (peer != nullptr) {

			if (_should_exit_thread) {
				if (!is_disconnecting) {
					enet_peer_disconnect(peer, 0);
					is_disconnecting = true;
					disconnect_start_time = enet_time_get();
				}
				else {
					if ((enet_time_get() - disconnect_start_time) > 1000) {
						trace("enet_peer_disconnect took too long");
						enet_peer_reset(peer);
						peer = nullptr;
						break;
					}
				}
			}

			if (!is_disconnecting) {
				send_queued_packets_in_thread(peer);
			}

			//flush / capture enet events
			//http://lists.cubik.org/pipermail/enet-discuss/2013-September/002240.html
			enet_host_service(host, 0, 0);
			{
				ENetEvent e;
				while (enet_host_check_events(host, &e) > 0) {
					std::lock_guard<std::mutex> lock(_event_queue_mutex);
					_event_queue.push(e);
					if (e.type == ENET_EVENT_TYPE_DISCONNECT) {
						trace("ENET_EVENT_TYPE_DISCONNECT received");
						peer = nullptr;
						break;
					}
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		enet_host_destroy(host);
	}

	void client::send_queued_packets_in_thread(ENetPeer* peer) {
		if (!_packet_queue.empty()) {
			std::lock_guard<std::mutex> lock(_packet_queue_mutex);
			while (!_packet_queue.empty()) {
				auto qp = _packet_queue.front();
				_packet_queue.pop();
				
				if (enet_peer_send(peer, qp._channel, qp._packet) != 0) {
					trace("enet_peer_send failed");
				}

				if (qp._packet->referenceCount == 0) {
					enet_packet_destroy(qp._packet);
				}
			}
		}
	}

	void client::trace(const std::string& s) {
		if (_trace_func != nullptr) {
			_trace_func(s);
		}
	}

}