
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
		static uint16_t _read_uint16_t(const byte_t* px);
		static uint32_t _read_uint32_t(const byte_t* px);
		static uint64_t _read_uint64_t(const byte_t* px);
		static void		_write_bynary_uint16_t(byte_t* out, const uint16_t value);
		static void		_write_bynary_uint32_t(byte_t* out, const uint32_t value);
		static void		_write_bynary_uint64_t(byte_t* out, const uint64_t value);
		static void		_copy_memory(void* _dst, const void* _src, const std::size_t byte_count);

		static std::size_t _serialize_packed_uint64_t(std::array<byte_t, 8>& buffer, const uint64_t value);
		static uint64_t _deserialize_packed_uint64_t(const std::array<byte_t, 8>& buffer, const uint8_t& sz);
	};

}
