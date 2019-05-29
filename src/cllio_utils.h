
#pragma once

#include "cllio_config.h"

#define CLLIO_UNUSED(V) ((const void*)&V)

namespace cllio
{
	//--------------------------------------------------------------------------------------------------------

	template <class F, class S>
	union UnionCast
	{
		F 	first;
		S 	second;
	};

	

}