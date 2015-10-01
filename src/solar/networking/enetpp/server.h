#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>
#include <queue>
#include <mutex>
#include "server_listen_params.h"
#include "server_queued_packet.h"
#include "server_peer_event.h"

namespace enetpp {

	class server {
	public:
		using thread_safe_trace_func = std::function<void(const std::string&)>;

	private:
		thread_safe_trace_func _trace_func;

		std::unique_ptr<std::thread> _thread;
		bool _should_exit_thread;

		std::unordered_map<unsigned int, ENetPeer*> _peer_map;

		std::queue<server_queued_packet> _packet_queue;
		std::mutex _packet_queue_mutex;

		std::queue<server_peer_event> _event_queue;
		std::queue<server_peer_event> _event_queue_copy; //member variable instead of stack to prevent mallocs?
		std::mutex _event_queue_mutex;

	public:
		server(thread_safe_trace_func trace_func);
		~server();

		bool is_listening() const;
		void start_listening(const server_listen_params& params);
		void stop_listening();

		void send_packet_async(enet_uint8 channel, ENetPacket* packet, unsigned int peer_uid);

		void consume_events(std::function<void(const server_peer_event&)> event_handler);

	private:
		void run_in_thread(const server_listen_params& params);
		void disconnect_all_peers_in_thread();
		void send_queued_packets_in_thread();
		void capture_events_in_thread(const server_listen_params& params, ENetHost* host);
		void handle_connect_event_in_thread(const server_listen_params& params, const ENetEvent& e);
		void handle_receive_event_in_thread(const ENetEvent& e);
		void handle_disconnect_event_in_thread(const ENetEvent& e);
		void trace(const std::string& s);
		void destroy_all_queued_packets();
		void destroy_all_queued_events();
		void destroy_event_data(server_peer_event& e);
	};

}