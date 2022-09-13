
#pragma once

#include "cllio_config.h"

namespace cllio
{
	using byte_t = uint8_t;

	//--------------------------------------------------------------------------------------------------------

	template <class F, class S>
	union UnionCast
	{
		F first;
		S second;
	};

	struct _serializer_utils
	{
		static void _write_bynary_uint16_t(byte_t* out, const uint16_t value);
		static void _write_bynary_uint32_t(byte_t* out, const uint32_t value);
		static void _write_bynary_uint64_t(byte_t* out, const uint64_t value);
		static void _copy_memory(void* _dst, const void* _src, const std::size_t byte_count);
	};

}
