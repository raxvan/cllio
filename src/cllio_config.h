
#pragma once

#ifdef CLLIO_FORCE_ASSERT

	#if CLLIO_FORCE_ASSERT == 0
	
		//disable asserts
		#define CLLIO_ASSERTS_ENABLED 0

	#elif CLLIO_FORCE_ASSERT == 1
		
		//enable asserts
		#define CLLIO_ASSERTS_ENABLED 1

	#elif CLLIO_FORCE_ASSERT == 2

		//enable with custom external handler
		#define CLLIO_ASSERTS_ENABLED 2
	
	#endif

#endif

#ifndef CLLIO_ASSERTS_ENABLED
	#ifdef DEBUG
		#define CLLIO_ASSERTS_ENABLED 1
	#else
		#define CLLIO_ASSERTS_ENABLED 0
	#endif
#endif



#if CLLIO_ASSERTS_ENABLED != 0
	namespace cllio
	{
		extern void cllio_assert_failed(const char * file, const int line, const char * cond);
	}
	#define CLLIO_ASSERT(_COND) do{ if(!(_COND)) cllio_assert_failed(__FILE__,__LINE__,#_COND); }while(false)
#else
	#define CLLIO_ASSERT(_COND) do{}while(false)
#endif

#include <cstdio> //for std::FILE, std::size_t
#include <cstdint> //for uint8_t, etc
#include <cstddef> //for std::ptrdiff_t, should optimize out the include and use std::size_t
