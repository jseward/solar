#pragma once

namespace solar {

	template<typename T> void d3d9_release_com_object(T& obj) {
		if (obj != nullptr) {
			obj->Release();
			obj = nullptr;
		}
	}

}