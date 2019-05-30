

#include "memory_stream_io.h"

namespace cllio
{

	memory_read_stream::memory_read_stream(const void * px)
		:m_px(static_cast<const byte_t*>(px))
	{
	}
	//-----------------------------------------------------------------------------------------------------------
	inline uint16_t _deserialize_uint16_t(const byte_t*& px)
	{
		CLLIO_ASSERT(px != nullptr);
		uint16_t r;
		r = (static_cast<uint16_t>(*px++));
		r |= (static_cast<uint16_t>(*px++) << 8);
		return r;
	}
	inline uint32_t _deserialize_uint32_t(const byte_t*& px)
	{
		CLLIO_ASSERT(px != nullptr);
		uint32_t r;
		r = (static_cast<uint32_t>(*px++));
		r |= (static_cast<uint32_t>(*px++) << 8);
		r |= (static_cast<uint32_t>(*px++) << 16);
		r |= (static_cast<uint32_t>(*px++) << 24);
		return r;
	}
	inline uint64_t _deserialize_uint64_t(const byte_t*& px)
	{
		CLLIO_ASSERT(px != nullptr);
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
	//-----------------------------------------------------------------------------------------------------------
	uint8_t		memory_read_stream::pop_uint8_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return *m_px++;
	}
	uint16_t	memory_read_stream::pop_uint16_t()
	{
		return _deserialize_uint16_t(m_px);
	}
	uint32_t	memory_read_stream::pop_uint32_t()
	{
		return _deserialize_uint32_t(m_px);
	}
	uint64_t	memory_read_stream::pop_uint64_t()
	{
		return _deserialize_uint64_t(m_px);
	}

	int8_t		memory_read_stream::pop_int8_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint8_t,int8_t> tmp;
		tmp.first = *m_px++;
		return tmp.second;
	}
	int16_t		memory_read_stream::pop_int16_t()
	{
		UnionCast<uint16_t,int16_t> tmp;
		tmp.first = _deserialize_uint16_t(m_px);
		return tmp.second;
	}
	int32_t		memory_read_stream::pop_int32_t()
	{
		UnionCast<uint32_t,int32_t> tmp;
		tmp.first = _deserialize_uint32_t(m_px);
		return tmp.second;
	}
	int64_t		memory_read_stream::pop_int64_t()
	{
		UnionCast<uint64_t,int64_t> tmp;
		tmp.first = _deserialize_uint64_t(m_px);
		return tmp.second;
	}

	float		memory_read_stream::pop_float()
	{
		UnionCast<uint32_t,float> tmp;
		tmp.first = _deserialize_uint32_t(m_px);
		return tmp.second;
	}
	double		memory_read_stream::pop_double()
	{
		UnionCast<uint64_t,double> tmp;
		tmp.first = _deserialize_uint64_t(m_px);
		return tmp.second;
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
}