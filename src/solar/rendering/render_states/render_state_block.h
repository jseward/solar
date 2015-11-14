#pragma once

namespace solar {

	class render_state_block {
	public:
		virtual void commit() = 0;
	};

}