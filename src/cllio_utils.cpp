
#include "cllio_utils.h"

#if CLLIO_ASSERTS_ENABLED == 1 // handle assert includes
#	include <iostream>
#	include <cassert>
#endif

namespace cllio
{
#if CLLIO_ASSERTS_ENABLED == 1
	void cllio_assert_failed(const char* file, const int line, const char* cond)
	{
		std::cout << "CLLIO_ASSERT failed in " << file << "(" << line << ")> " << cond << std::endl;
		assert(false);
	}
#endif
}