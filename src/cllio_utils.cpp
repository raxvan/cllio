
#include "cllio_utils.h"

#if CLLIO_ASSERTS_ENABLED == 1 
	//handle assert includes
	#include <iostream>
#endif

namespace cllio
{
#if CLLIO_ASSERTS_ENABLED == 1
	void cllio_assert_failed(const char * file, const int line, const char * cond)
	{
		std::cout << "CLLIO_ASSERT failed in " << file << "(" << line << ")> " << cond << std::endl;
		assert(false);
	}
#endif

#if CLLIO_CAST_FUNCTION == 0

	void detail::uint16_to_binary(buffer<2,uint16_t>& storage, const uint16_t value)
	{
		storage.data[0] = static_cast<uint8_t>(value & 0xFF);
		storage.data[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
	}
	void detail::uint32_to_binary(buffer<4,uint32_t>& storage, const uint32_t value)
	{
		storage.data[0] = static_cast<uint8_t>(value & 0xFF);
		storage.data[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
		storage.data[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
		storage.data[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
	}
	void detail::uint64_to_binary(buffer<8,uint64_t>& storage, const uint64_t value)
	{
		storage.data[0] = static_cast<uint8_t>(value & 0xFF);
		storage.data[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
		storage.data[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
		storage.data[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
		storage.data[4] = static_cast<uint8_t>((value >> 32) & 0xFF);
		storage.data[5] = static_cast<uint8_t>((value >> 40) & 0xFF);
		storage.data[6] = static_cast<uint8_t>((value >> 48) & 0xFF);
		storage.data[7] = static_cast<uint8_t>((value >> 56) & 0xFF);
	}

	uint16_t detail::uint16_from_binary(const buffer<2,uint16_t>& storage)
	{
		uint16_t r = 0;
		r |= (static_cast<uint16_t>(storage.data[0]));
		r |= (static_cast<uint16_t>(storage.data[1]) << 8);
		return r;
	}
	uint32_t detail::uint32_from_binary(const buffer<4,uint32_t>& storage)
	{
		uint32_t r = 0;
		r |= (static_cast<uint32_t>(storage.data[0]));
		r |= (static_cast<uint32_t>(storage.data[1]) << 8);
		r |= (static_cast<uint32_t>(storage.data[2]) << 16);
		r |= (static_cast<uint32_t>(storage.data[3]) << 24);
		return r;
	}
	uint64_t detail::uint64_from_binary(const buffer<8,uint64_t>& storage)
	{
		uint64_t r = 0;
		r |= (static_cast<uint64_t>(storage.data[0]));
		r |= (static_cast<uint64_t>(storage.data[1]) << 8);
		r |= (static_cast<uint64_t>(storage.data[2]) << 16);
		r |= (static_cast<uint64_t>(storage.data[3]) << 24);
		r |= (static_cast<uint64_t>(storage.data[4]) << 32);
		r |= (static_cast<uint64_t>(storage.data[5]) << 40);
		r |= (static_cast<uint64_t>(storage.data[6]) << 48);
		r |= (static_cast<uint64_t>(storage.data[7]) << 56);
		return r;
	}
#endif
}