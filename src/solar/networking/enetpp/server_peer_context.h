#pragma once

namespace enetpp {

	class server_peer_context {
	public:
		virtual unsigned int get_uid() const = 0;
	};

}