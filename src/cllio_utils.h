
#pragma once

#include "cllio_config.h"

//#define CLLIO_UNUSED(V) ((const void*)&V)

namespace cllio
{
	using byte_t = uint8_t;
	
	//--------------------------------------------------------------------------------------------------------

	template <class F, class S>
	union UnionCast
	{
		F 	first;
		S 	second;
	};

	template <class T>
	void consider_it_used(const T & v)
	{
		//unused variable bypass
		((const void*)&v);
	}
	

}