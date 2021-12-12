
#include "cllio_utils.h"

#ifdef CLLIO_ASSERT_ENABLED
#	include <iostream>
#	include <cassert>
#endif

namespace cllio
{
#ifdef CLLIO_ASSERT_ENABLED
	void cllio_assert_failed(const char* file, const int line, const char* cond)
	{
		std::cerr << "CLLIO_ASSERT failed in " << file << "(" << line << ")> " << cond << std::endl;
		assert(false);
	}
#endif
}