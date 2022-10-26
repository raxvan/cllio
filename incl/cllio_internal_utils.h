
#pragma once

#include "cllio_config.h"

namespace cllio
{
	using byte_t = uint8_t;

	//--------------------------------------------------------------------------------------------------------

	struct _serializer_utils
	{
		static uint16_t _read_uint16_t(const byte_t* px);
		static uint32_t _read_uint32_t(const byte_t* px);
		static uint64_t _read_uint64_t(const byte_t* px);
		static void _write_bynary_uint16_t(byte_t* out, const uint16_t value);
		static void _write_bynary_uint32_t(byte_t* out, const uint32_t value);
		static void _write_bynary_uint64_t(byte_t* out, const uint64_t value);
		static void _copy_memory(void* _dst, const void* _src, const std::size_t byte_count);


		//signed to unsigned cast
		static uint8_t _stu8(const int8_t);
		static uint16_t _stu16(const int16_t);
		static uint32_t _stu32(const int32_t);
		static uint64_t _stu64(const int64_t);

		//unsigned to signed cast
		static int8_t _uts8(const uint8_t);
		static int16_t _uts16(const uint16_t);
		static int32_t _uts32(const uint32_t);
		static int64_t _uts64(const uint64_t);

		//float casts:
		static float _utf(const uint32_t);
		static double _utd(const uint64_t);

		static uint32_t _ftu(const float);
		static uint64_t _dtu(const double);

		//pts:

		static uint64_t _ptu(const void* p);
		
	};

}
