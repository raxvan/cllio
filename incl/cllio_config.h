
#pragma once

//--------------------------------------------------------------------------------------------------------------------------------

#ifdef CLLIO_USE_DEV_PLATFORM

#	include <devtiny.h>
#	define CLLIO_ASSERT DEV_ASSERT

#elif defined(CLLIO_TESTING)
#	include <ttf.h>
#	define CLLIO_ASSERT TTF_ASSERT

#elif defined(CLLIO_ENABLE_ASSERT)

namespace cllio
{
	extern "C++" void cllio_assert_failed(const char* file, const int line, const char* cond);
}
#	define CLLIO_ASSERT(_COND)                                  \
		do                                                       \
		{                                                        \
			if (!(_COND))                                        \
				cllio_assert_failed(__FILE__, __LINE__, #_COND); \
		} while (false)
#	define CLLIO_ASSERT_FALSE(CSTR_MSG)                       \
		do                                                     \
		{                                                      \
			cllio_assert_failed(__FILE__, __LINE__, CSTR_MSG); \
		} while (false)
#endif

#ifndef CLLIO_ASSERT
#	define CLLIO_ASSERT(...) \
		do                    \
		{                     \
		} while (false)
#endif

#ifndef CLLIO_ASSERT_FALSE
#	define CLLIO_ASSERT_FALSE(...) \
		do                          \
		{                           \
		} while (false)
#endif
//--------------------------------------------------------------------------------------------------------------------------------

#include <cstdio>  //for std::FILE, std::size_t
#include <cstdint> //for uint8_t, etc
#include <cstddef> //for std::ptrdiff_t, i should optimize out the include and use std::size_t
#include <utility> //for std::forward
