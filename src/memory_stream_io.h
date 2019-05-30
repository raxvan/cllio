#pragma once
#include "cllio_utils.h"

namespace cllio
{

	struct memory_read_stream
	{
	protected:
		const byte_t* m_px = nullptr;

	public:
		memory_read_stream(const void * px);
		
		memory_read_stream() = default;
		memory_read_stream(const memory_read_stream &) = default;
		memory_read_stream& operator = (const memory_read_stream &) = default;
	public:
		uint8_t		pop_uint8_t();
		uint16_t	pop_uint16_t();
		uint32_t	pop_uint32_t();
		uint64_t	pop_uint64_t();

		int8_t		pop_int8_t();
		int16_t		pop_int16_t();
		int32_t		pop_int32_t();
		int64_t		pop_int64_t();

		float		pop_float();
		double		pop_double();
	};

}