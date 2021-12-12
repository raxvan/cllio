
#pragma once

//--------------------------------------------------------------------------------------------------------------------------------

#if defined(CLLIO_INSTRUMENTS) || defined(TESTING_BUILD) //enable assert
#define CLLIO_ASSERT_ENABLED
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
#	define CLLIO_ASSERT_FALSE(CSTR_MSG)                          \
		do                                                       \
		{                                                        \
			cllio_assert_failed(__FILE__, __LINE__, CSTR_MSG);   \
		} while (false)
#else
#define CLLIO_ASSERT(...) \
		do                      \
		{                       \
		} while (false)
#define CLLIO_ASSERT_FALSE(...) \
		do                      \
		{                       \
		} while (false)
#endif

//--------------------------------------------------------------------------------------------------------------------------------

#include <cstdio>  //for std::FILE, std::size_t
#include <cstdint> //for uint8_t, etc
#include <cstddef> //for std::ptrdiff_t, i should optimize out the include and use std::size_t
#include <utility> //for std::forward
