#include "d3d9_error.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	std::string make_d3d9_error_message(HRESULT hr) {
		UNUSED_PARAMETER(hr);

		//http://stackoverflow.com/questions/31053670/unresolved-external-symbol-vsnprintf-in-dxerr-lib
		//can't use dxerr functions. ex. DXGetErrorDescriptionA

		return "???"; //todo
	}

}