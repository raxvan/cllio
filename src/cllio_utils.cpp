
#include "cllio_utils.h"

#ifdef CLLIO_ENABLE_ASSERT
#	include <iostream>
#	include <cassert>
#endif

namespace cllio
{
#ifdef CLLIO_ENABLE_ASSERT
	void cllio_assert_failed(const char* file, const int line, const char* cond)
	{
		std::cerr << "CLLIO_ASSERT failed in " << file << "(" << line << ")> " << cond << std::endl;
		assert(false);
	}
#endif
}