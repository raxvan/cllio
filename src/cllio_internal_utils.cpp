
#include <cllio.h>

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

	inline byte_t base16ToBase10(const char c)
	{
		if (c >= '0' && c <= '9')
			return byte_t(c - '0');
		else if (c >= 'a' && c <= 'f')
			return byte_t(c - 'a') + 10;
		else if (c >= 'A' && c <= 'F')
			return byte_t(c - 'F') + 10;
		else
		{
			CLLIO_ASSERT(false);
			return 0;
		}
	}

	byte_t utils::fromhex(char first, char second)
	{
		byte_t f = base16ToBase10(first);
		byte_t s = base16ToBase10(second);
		return f << 4 | s;
	}

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

	template <class F, class S>
	union UnionCast
	{
		F first;
		S second;
	};

	uint16_t _serializer_utils::_read_uint16_t(const byte_t* px)
	{
		uint16_t r;
		r = (static_cast<uint16_t>(*px++));
		r |= (static_cast<uint16_t>(*px) << uint16_t(8));
		return r;
	}
	uint32_t _serializer_utils::_read_uint32_t(const byte_t* px)
	{
		uint32_t r;
		r = (static_cast<uint32_t>(*px++));
		r |= (static_cast<uint32_t>(*px++) << 8);
		r |= (static_cast<uint32_t>(*px++) << 16);
		r |= (static_cast<uint32_t>(*px) << 24);
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
		r |= (static_cast<uint64_t>(*px) << 56);
		return r;
	}

	uint8_t _serializer_utils::_stu8(const int8_t value)
	{
		UnionCast<uint8_t, int8_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	uint16_t _serializer_utils::_stu16(const int16_t value)
	{
		UnionCast<uint16_t, int16_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	uint32_t _serializer_utils::_stu32(const int32_t value)
	{
		UnionCast<uint32_t, int32_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	uint64_t _serializer_utils::_stu64(const int64_t value)
	{
		UnionCast<uint64_t, int64_t> tmp;
		tmp.first = value;
		return tmp.second;
	}

	int8_t _serializer_utils::_uts8(const uint8_t value)
	{
		UnionCast<int8_t, uint8_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	int16_t _serializer_utils::_uts16(const uint16_t value)
	{
		UnionCast<int16_t, uint16_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	int32_t _serializer_utils::_uts32(const uint32_t value)
	{
		UnionCast<int32_t, uint32_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	int64_t _serializer_utils::_uts64(const uint64_t value)
	{
		UnionCast<int64_t, uint64_t> tmp;
		tmp.first = value;
		return tmp.second;
	}

	float _serializer_utils::_utf(const uint32_t value)
	{
		UnionCast<uint32_t, float> tmp;
		tmp.first = value;
		return tmp.second;
	}
	double _serializer_utils::_utd(const uint64_t value)
	{
		UnionCast<uint64_t, double> tmp;
		tmp.first = value;
		return tmp.second;
	}

	uint32_t _serializer_utils::_ftu(const float value)
	{
		UnionCast<float, uint32_t> tmp;
		tmp.first = value;
		return tmp.second;
	}
	uint64_t _serializer_utils::_dtu(const double value)
	{
		UnionCast<double, uint64_t> tmp;
		tmp.first = value;
		return tmp.second;
	}

	uint64_t _serializer_utils::_ptu(const void* px)
	{
		UnionCast<uint64_t, const void*> tmp;
		tmp.first = 0;
		tmp.second = px;
		return tmp.first;
	}

}
