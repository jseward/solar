#pragma once

#include "solar/utility/exe_runner.h"

namespace solar {

	class win32_exe_runner : public exe_runner {
	public:
		virtual run_exe_result run_exe(const std::string& exe_path, const std::string& arguments) override;
	};

}
