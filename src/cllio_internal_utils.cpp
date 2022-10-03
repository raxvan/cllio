
#include <cllio_internal_utils.h>

#ifdef CLLIO_ASSERT_ENABLED
#	include <iostream>
#	include <cassert>
#endif
#include <cstring>

namespace cllio
{
#ifdef CLLIO_ASSERT_ENABLED
	void cllio_assert_failed(const char* file, const int line, const char* cond)
	{
		std::cerr << "CLLIO_ASSERT failed in " << file << "(" << line << ")> " << cond << std::endl;
		assert(false);
	}
#endif

	void _serializer_utils::_write_bynary_uint16_t(byte_t* out, const uint16_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out = static_cast<uint8_t>((value >> 8) & 0xFF);
	}
	void _serializer_utils::_write_bynary_uint32_t(byte_t* out, const uint32_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
		*out = static_cast<uint8_t>((value >> 24) & 0xFF);
	}
	void _serializer_utils::_write_bynary_uint64_t(byte_t* out, const uint64_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 24) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 32) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 40) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 48) & 0xFF);
		*out = static_cast<uint8_t>((value >> 56) & 0xFF);
	}

	void _serializer_utils::_copy_memory(void* _dst, const void* _src, const std::size_t byte_count)
	{
		CLLIO_ASSERT(_dst != nullptr && _src != nullptr);
		std::memcpy(_dst, _src, byte_count);
	}

	uint16_t _serializer_utils::_read_uint16_t(const byte_t* px)
	{
		uint16_t r;
		r = (static_cast<uint16_t>(*px++));
		r |= (static_cast<uint16_t>(*px++) << uint16_t(8));
		return r;
	}
	uint32_t _serializer_utils::_read_uint32_t(const byte_t* px)
	{
		uint32_t r;
		r = (static_cast<uint32_t>(*px++));
		r |= (static_cast<uint32_t>(*px++) << 8);
		r |= (static_cast<uint32_t>(*px++) << 16);
		r |= (static_cast<uint32_t>(*px++) << 24);
		return r;
	}
	uint64_t _serializer_utils::_read_uint64_t(const byte_t* px)
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

	std::size_t _serializer_utils::_serialize_packed_uint64_t(std::array<byte_t, 8>& buffer, const uint64_t value)
	{
		const uint8_t low_mask = (1 << 5) - 1;

		if (value <= low_mask) // first 5 bits
		{
			buffer[7] = byte_t(value);
			return 7;
		}

		std::size_t sz = 8;
		{
			//put bytes to buffer, starting from 1
			uint64_t tmp = value;
			while (tmp != 0)
			{
				buffer[--sz] = tmp & 255;
				tmp = tmp >> 8;
			}
		}
		if(buffer[sz] > low_mask)
		{
			//overflow
			if(sz == 0)
				return 8;
			buffer[--sz] = 0;
		}
		buffer[sz] |= uint8_t(8 - sz - 1) << 5;
		return sz;
	}

	uint64_t _serializer_utils::_deserialize_packed_uint64_t(const std::array<byte_t, 8>& buffer, const uint8_t& sz)
	{
		const uint8_t low_mask = (1 << 5) - 1;

		uint64_t r = buffer[0] & low_mask;
		for(std::size_t i = 0, s = sz; i < s;i++)
			r = (r << 8) | buffer[i + 1];
		return r;
	}

}
