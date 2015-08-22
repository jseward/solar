#pragma once

#define MAKE_PROPERTY(CLASS_NAME, PARAM_TYPE, PARAM_NAME, DEFAULT_VALUE)\
	private:\
	PARAM_TYPE _##PARAM_NAME = DEFAULT_VALUE;\
	public:\
	CLASS_NAME& set_##PARAM_NAME(PARAM_TYPE PARAM_NAME) {\
		_##PARAM_NAME = PARAM_NAME;\
		return *this;\
	}\
	const PARAM_TYPE& get_##PARAM_NAME() const {\
		return _##PARAM_NAME;\
	}\
	PARAM_TYPE& get_##PARAM_NAME() {\
		return _##PARAM_NAME;\
	}

#define MAKE_PROPERTY_AS_COPY(CLASS_NAME, PARAM_TYPE, PARAM_NAME, DEFAULT_VALUE)\
	private:\
	PARAM_TYPE _##PARAM_NAME = DEFAULT_VALUE;\
	public:\
	CLASS_NAME& set_##PARAM_NAME(PARAM_TYPE PARAM_NAME) {\
		_##PARAM_NAME = PARAM_NAME;\
		return *this;\
	}\
	PARAM_TYPE get_##PARAM_NAME() const {\
		return _##PARAM_NAME;\
	}
