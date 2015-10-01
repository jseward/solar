#pragma once

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include "client_connect_params.h"
#include "client_queued_packet.h"

namespace enetpp {

	class client {
	public:
		using thread_safe_trace_func = std::function<void(const std::string&)>;

	private:
		thread_safe_trace_func _trace_func;

		std::queue<client_queued_packet> _packet_queue;
		std::mutex _packet_queue_mutex;

		std::queue<ENetEvent> _event_queue;
		std::queue<ENetEvent> _event_queue_copy; //member variable instead of stack to prevent mallocs?
		std::mutex _event_queue_mutex;

		bool _should_exit_thread;
		std::unique_ptr<std::thread> _thread;

	public:
		client(thread_safe_trace_func trace_func);
		~client();

		bool is_connecting_or_connected() const;
		void connect(const client_connect_params& params);
		void disconnect();

		void send_packet_async(enet_uint8 channel, ENetPacket* packet);

		void consume_events(std::function<void(const ENetEvent&)> event_handler);

	private:
		void run_in_thread(const client_connect_params& params);
		void send_queued_packets_in_thread(ENetPeer* peer);
		void trace(const std::string& s);
		void destroy_all_queued_packets();
		void destroy_all_queued_events();
		void destroy_event_data(ENetEvent& e);
	};

}