
#pragma once

#include "cllio_config.h"
#include <cstdarg> //for va_start
#include <cstring> //for std::memcpy


namespace cllio
{
	template <class T>
	inline void consider_it_used(const T& v)
	{
		// unused variable bypass
		((void)v);
	}

	struct _inline_serializer_utils
	{
		inline static uint16_t _read_uint16_t(const byte_t*& px)
		{

			uint16_t r;
			r = (static_cast<uint16_t>(*px++));
			r |= (static_cast<uint16_t>(*px++) << uint16_t(8));
			return r;
		}
		inline static uint32_t _read_uint32_t(const byte_t*& px)
		{
			uint32_t r;
			r = (static_cast<uint32_t>(*px++));
			r |= (static_cast<uint32_t>(*px++) << 8);
			r |= (static_cast<uint32_t>(*px++) << 16);
			r |= (static_cast<uint32_t>(*px++) << 24);
			return r;
		}
		inline static uint64_t _read_uint64_t(const byte_t*& px)
		{
			uint64_t r;
			r = (static_cast<uint64_t>(*px++));
			r |= (static_cast<uint64_t>(*px++) << 8);
			r |= (static_cast<uint64_t>(*px++) << 16);
			r |= (static_cast<uint64_t>(*px++) << 24);
			r |= (static_cast<uint64_t>(*px++) << 32);
			r |= (static_cast<uint64_t>(*px++) << 40);
			r |= (static_cast<uint64_t>(*px++) << 48);
			r |= (static_cast<uint64_t>(*px++) << 56);
			return r;
		}

		inline static void _write_bynary_uint16_t(byte_t*& out, const uint16_t value)
		{
			*out++ = static_cast<uint8_t>(value & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
		}
		inline static void _write_bynary_uint32_t(byte_t*& out, const uint32_t value)
		{
			*out++ = static_cast<uint8_t>(value & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 24) & 0xFF);
		}
		inline static void _write_bynary_uint64_t(byte_t*& out, const uint64_t value)
		{
			*out++ = static_cast<uint8_t>(value & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 24) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 32) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 40) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 48) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 56) & 0xFF);
		}

		inline static int16_t _read_int16_t(const byte_t*& px)
		{
			return _serializer_utils::_uts16(_inline_serializer_utils::_read_uint16_t(px));
		}
		inline static int32_t _read_int32_t(const byte_t*& px)
		{
			return _serializer_utils::_uts32(_inline_serializer_utils::_read_uint32_t(px));
		}
		inline static int64_t _read_int64_t(const byte_t*& px)
		{
			return _serializer_utils::_uts64(_inline_serializer_utils::_read_uint64_t(px));
		}
		inline static float _read_float(const byte_t*& px)
		{
			return _serializer_utils::_utf(_inline_serializer_utils::_read_uint32_t(px));
		}
		inline static double _read_double(const byte_t*& px)
		{
			return _serializer_utils::_utd(_inline_serializer_utils::_read_uint64_t(px));
		}
	};

}
