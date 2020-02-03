

#include "memory_stream_io.h"
#include "cllio_internal.h"

namespace cllio
{
	inline uint16_t _read_uint16_t(const byte_t*& px)
	{

		uint16_t r;
		r = (static_cast<uint16_t>(*px++));
		r |= (static_cast<uint16_t>(*px++) << 8);
		return r;
	}
	inline uint32_t _read_uint32_t(const byte_t*& px)
	{
		uint32_t r;
		r = (static_cast<uint32_t>(*px++));
		r |= (static_cast<uint32_t>(*px++) << 8);
		r |= (static_cast<uint32_t>(*px++) << 16);
		r |= (static_cast<uint32_t>(*px++) << 24);
		return r;
	}
	inline uint64_t _read_uint64_t(const byte_t*& px)
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

	inline void _write_bynary_uint16_t(byte_t*& out, const uint16_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
	}
	inline void _write_bynary_uint32_t(byte_t*& out, const uint32_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 24) & 0xFF);
	}
	inline void _write_bynary_uint64_t(byte_t*& out, const uint64_t value)
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

	void _memory_functor_write_details::_write_bynary_uint16_t(byte_t* out, const uint16_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out = static_cast<uint8_t>((value >> 8) & 0xFF);
	}
	void _memory_functor_write_details::_write_bynary_uint32_t(byte_t* out, const uint32_t value)
	{
		*out++ = static_cast<uint8_t>(value & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
		*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
		*out = static_cast<uint8_t>((value >> 24) & 0xFF);
	}
	void _memory_functor_write_details::_write_bynary_uint64_t(byte_t* out, const uint64_t value)
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


	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------


	mem_stream_read_unchecked::mem_stream_read_unchecked(const void* px)
		: m_px(static_cast<const byte_t*>(px))
	{
	}
	//-----------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------
	uint8_t mem_stream_read_unchecked::pop_uint8_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return *m_px++;
	}
	uint16_t mem_stream_read_unchecked::pop_uint16_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _read_uint16_t(m_px);
	}
	uint32_t mem_stream_read_unchecked::pop_uint32_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _read_uint32_t(m_px);
	}
	uint64_t mem_stream_read_unchecked::pop_uint64_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _read_uint64_t(m_px);
	}

	int8_t mem_stream_read_unchecked::pop_int8_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint8_t, int8_t> tmp;
		tmp.first = *m_px++;
		return tmp.second;
	}
	int16_t mem_stream_read_unchecked::pop_int16_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint16_t, int16_t> tmp;
		tmp.first = _read_uint16_t(m_px);
		return tmp.second;
	}
	int32_t mem_stream_read_unchecked::pop_int32_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, int32_t> tmp;
		tmp.first = _read_uint32_t(m_px);
		return tmp.second;
	}
	int64_t mem_stream_read_unchecked::pop_int64_t()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, int64_t> tmp;
		tmp.first = _read_uint64_t(m_px);
		return tmp.second;
	}

	float mem_stream_read_unchecked::pop_float()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, float> tmp;
		tmp.first = _read_uint32_t(m_px);
		return tmp.second;
	}
	double mem_stream_read_unchecked::pop_double()
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, double> tmp;
		tmp.first = _read_uint64_t(m_px);
		return tmp.second;
	}
	//-----------------------------------------------------------------------------------------------------------
	mem_stream_read::mem_stream_read(const void* px, const std::size_t size)
		: m_px(static_cast<const uint8_t*>(px))
		, m_px_end(static_cast<const uint8_t*>(px) + size)
	{
	}
	std::ptrdiff_t mem_stream_read::size() const
	{
		CLLIO_ASSERT(m_px <= m_px_end);
		return m_px_end - m_px;
	}
	//-----------------------------------------------------------------------------------------------------------
	void mem_stream_read::buffer_read(void* dest, const std::size_t ammount)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + ammount) <= m_px_end);
		uint8_t* dst8 = static_cast<uint8_t*>(dest);
#ifdef _MSC_VER
		errno_t err = memcpy_s(dst8, ammount, m_px, ammount);
		CLLIO_ASSERT(err == 0);
		consider_it_used(err);
#else
		std::memcpy(dst8, m_px, ammount);
#endif
		m_px += ammount;
	}

	bool mem_stream_read::pop_uint8_t(uint8_t& out)
	{
		if ((m_px + sizeof(uint8_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		out = *m_px++;
		return true;
	}
	bool mem_stream_read::pop_uint16_t(uint16_t& out)
	{
		if ((m_px + sizeof(uint16_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		out = _read_uint16_t(m_px);
		return true;
	}
	bool mem_stream_read::pop_uint32_t(uint32_t& out)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		out = _read_uint32_t(m_px);
		return true;
	}
	bool mem_stream_read::pop_uint64_t(uint64_t& out)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		out = _read_uint64_t(m_px);
		return true;
	}

	bool mem_stream_read::pop_int8_t(int8_t& out)
	{
		if ((m_px + sizeof(uint8_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint8_t, int8_t> tmp;
		tmp.first = *m_px++;
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::pop_int16_t(int16_t& out)
	{
		if ((m_px + sizeof(uint16_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint16_t, int16_t> tmp;
		tmp.first = _read_uint16_t(m_px);
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::pop_int32_t(int32_t& out)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, int32_t> tmp;
		tmp.first = _read_uint32_t(m_px);
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::pop_int64_t(int64_t& out)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, int64_t> tmp;
		tmp.first = _read_uint64_t(m_px);
		out = tmp.second;
		return true;
	}

	bool mem_stream_read::pop_float(float& out)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, float> tmp;
		tmp.first = _read_uint32_t(m_px);
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::pop_double(double& out)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
			return false;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, double> tmp;
		tmp.first = _read_uint64_t(m_px);
		out = tmp.second;
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------

	uint8_t mem_stream_read::pop_uint8_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint8_t)) <= m_px_end);
		return *m_px++;
	}
	uint16_t mem_stream_read::pop_uint16_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint16_t)) <= m_px_end);
		return _read_uint16_t(m_px);
	}
	uint32_t mem_stream_read::pop_uint32_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint32_t)) <= m_px_end);
		return _read_uint32_t(m_px);
	}
	uint64_t mem_stream_read::pop_uint64_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint64_t)) <= m_px_end);
		return _read_uint64_t(m_px);
	}

	int8_t mem_stream_read::pop_int8_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint8_t)) <= m_px_end);
		UnionCast<uint8_t, int8_t> tmp;
		tmp.first = *m_px++;
		return tmp.second;
	}
	int16_t mem_stream_read::pop_int16_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint16_t)) <= m_px_end);
		UnionCast<uint16_t, int16_t> tmp;
		tmp.first = _read_uint16_t(m_px);
		return tmp.second;
	}
	int32_t mem_stream_read::pop_int32_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint32_t)) <= m_px_end);
		UnionCast<uint32_t, int32_t> tmp;
		tmp.first = _read_uint32_t(m_px);
		return tmp.second;
	}
	int64_t mem_stream_read::pop_int64_t()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint64_t)) <= m_px_end);
		UnionCast<uint64_t, int64_t> tmp;
		tmp.first = _read_uint64_t(m_px);
		return tmp.second;
	}

	float mem_stream_read::pop_float()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint32_t)) <= m_px_end);
		UnionCast<uint32_t, float> tmp;
		tmp.first = _read_uint32_t(m_px);
		return tmp.second;
	}
	double mem_stream_read::pop_double()
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint64_t)) <= m_px_end);
		UnionCast<uint64_t, double> tmp;
		tmp.first = _read_uint64_t(m_px);
		return tmp.second;
	}

	//-----------------------------------------------------------------------------------------------------------

	uint8_t mem_stream_read::popdefault_uint8_t(const uint8_t _default)
	{
		if ((m_px + sizeof(uint8_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		return *m_px++;
	}
	uint16_t mem_stream_read::popdefault_uint16_t(const uint16_t _default)
	{
		if ((m_px + sizeof(uint16_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		return _read_uint16_t(m_px);
	}
	uint32_t mem_stream_read::popdefault_uint32_t(const uint32_t _default)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		return _read_uint32_t(m_px);
	}
	uint64_t mem_stream_read::popdefault_uint64_t(const uint64_t _default)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		return _read_uint64_t(m_px);
	}

	int8_t mem_stream_read::popdefault_int8_t(const int8_t _default)
	{
		if ((m_px + sizeof(uint8_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint8_t, int8_t> tmp;
		tmp.first = *m_px++;
		return tmp.second;
	}
	int16_t mem_stream_read::popdefault_int16_t(const int16_t _default)
	{
		if ((m_px + sizeof(uint16_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint16_t, int16_t> tmp;
		tmp.first = _read_uint16_t(m_px);
		return tmp.second;
	}
	int32_t mem_stream_read::popdefault_int32_t(const int32_t _default)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, int32_t> tmp;
		tmp.first = _read_uint32_t(m_px);
		return tmp.second;
	}
	int64_t mem_stream_read::popdefault_int64_t(const int64_t _default)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, int64_t> tmp;
		tmp.first = _read_uint64_t(m_px);
		return tmp.second;
	}

	float mem_stream_read::popdefault_float(const float _default)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, float> tmp;
		tmp.first = _read_uint32_t(m_px);
		return tmp.second;
	}
	double mem_stream_read::popdefault_double(const double _default)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
			return _default;
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, double> tmp;
		tmp.first = _read_uint64_t(m_px);
		return tmp.second;
	}

	bool mem_stream_read::popdefault_uint8_t(uint8_t& out, const uint8_t _default)
	{
		if ((m_px + sizeof(uint8_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		out = *m_px++;
		return true;
	}
	bool mem_stream_read::popdefault_uint16_t(uint16_t& out, const uint16_t _default)
	{
		if ((m_px + sizeof(uint16_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		out = _read_uint16_t(m_px);
		return true;
	}
	bool mem_stream_read::popdefault_uint32_t(uint32_t& out, const uint32_t _default)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		out = _read_uint32_t(m_px);
		return true;
	}
	bool mem_stream_read::popdefault_uint64_t(uint64_t& out, const uint64_t _default)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		out = _read_uint64_t(m_px);
		return true;
	}

	bool mem_stream_read::popdefault_int8_t(int8_t& out, const int8_t _default)
	{
		if ((m_px + sizeof(uint8_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint8_t, int8_t> tmp;
		tmp.first = *m_px++;
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::popdefault_int16_t(int16_t& out, const int16_t _default)
	{
		if ((m_px + sizeof(uint16_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint16_t, int16_t> tmp;
		tmp.first = _read_uint16_t(m_px);
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::popdefault_int32_t(int32_t& out, const int32_t _default)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, int32_t> tmp;
		tmp.first = _read_uint32_t(m_px);
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::popdefault_int64_t(int64_t& out, const int64_t _default)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, int64_t> tmp;
		tmp.first = _read_uint64_t(m_px);
		out = tmp.second;
		return true;
	}

	bool mem_stream_read::popdefault_float(float& out, const float _default)
	{
		if ((m_px + sizeof(uint32_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint32_t, float> tmp;
		tmp.first = _read_uint32_t(m_px);
		out = tmp.second;
		return true;
	}
	bool mem_stream_read::popdefault_double(double& out, const double _default)
	{
		if ((m_px + sizeof(uint64_t)) > m_px_end)
		{
			out = _default;
			return false;
		}
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<uint64_t, double> tmp;
		tmp.first = _read_uint64_t(m_px);
		out = tmp.second;
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------

	mem_stream_write_unchecked::mem_stream_write_unchecked(void* px)
		: m_px(static_cast<uint8_t*>(px))
	{
	}

	void mem_stream_write_unchecked::push_int8_t(const int8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<int8_t, uint8_t> tmp;
		tmp.first = value;
		*m_px++ = tmp.second;
	}
	void mem_stream_write_unchecked::push_int16_t(const int16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<int16_t, uint16_t> tmp;
		tmp.first = value;
		_write_bynary_uint16_t(m_px, tmp.second);
	}
	void mem_stream_write_unchecked::push_int32_t(const int32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<int32_t, uint32_t> tmp;
		tmp.first = value;
		_write_bynary_uint32_t(m_px, tmp.second);
	}
	void mem_stream_write_unchecked::push_int64_t(const int64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<int64_t, uint64_t> tmp;
		tmp.first = value;
		_write_bynary_uint64_t(m_px, tmp.second);
	}

	void mem_stream_write_unchecked::push_uint8_t(const uint8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		*m_px++ = value;
	}
	void mem_stream_write_unchecked::push_uint16_t(const uint16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_write_bynary_uint16_t(m_px, value);
	}
	void mem_stream_write_unchecked::push_uint32_t(const uint32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_write_bynary_uint32_t(m_px, value);
	}
	void mem_stream_write_unchecked::push_uint64_t(const uint64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_write_bynary_uint64_t(m_px, value);
	}

	void mem_stream_write_unchecked::push_float(const float value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<float, uint32_t> tmp;
		tmp.first = value;
		_write_bynary_uint32_t(m_px, tmp.second);
	}
	void mem_stream_write_unchecked::push_double(const double value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		UnionCast<double, uint64_t> tmp;
		tmp.first = value;
		_write_bynary_uint64_t(m_px, tmp.second);
	}

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	mem_stream_write::mem_stream_write(void* px, const std::size_t size_in_bytes)
		: m_px(static_cast<uint8_t*>(px))
		, m_px_end(static_cast<uint8_t*>(px) + size_in_bytes)
	{
	}

	void mem_stream_write::push_int8_t(const int8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint8_t)) <= m_px_end);
		UnionCast<int8_t, uint8_t> tmp;
		tmp.first = value;
		*m_px++ = tmp.second;
	}
	void mem_stream_write::push_int16_t(const int16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint16_t)) <= m_px_end);
		UnionCast<int16_t, uint16_t> tmp;
		tmp.first = value;
		_write_bynary_uint16_t(m_px, tmp.second);
	}
	void mem_stream_write::push_int32_t(const int32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint32_t)) <= m_px_end);
		UnionCast<int32_t, uint32_t> tmp;
		tmp.first = value;
		_write_bynary_uint32_t(m_px, tmp.second);
	}
	void mem_stream_write::push_int64_t(const int64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint64_t)) <= m_px_end);
		UnionCast<int64_t, uint64_t> tmp;
		tmp.first = value;
		_write_bynary_uint64_t(m_px, tmp.second);
	}

	void mem_stream_write::push_uint8_t(const uint8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint8_t)) <= m_px_end);
		*m_px++ = value;
	}
	void mem_stream_write::push_uint16_t(const uint16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint16_t)) <= m_px_end);
		_write_bynary_uint16_t(m_px, value);
	}
	void mem_stream_write::push_uint32_t(const uint32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint32_t)) <= m_px_end);
		_write_bynary_uint32_t(m_px, value);
	}
	void mem_stream_write::push_uint64_t(const uint64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint64_t)) <= m_px_end);
		_write_bynary_uint64_t(m_px, value);
	}

	void mem_stream_write::push_float(const float value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint32_t)) <= m_px_end);
		UnionCast<float, uint32_t> tmp;
		tmp.first = value;
		_write_bynary_uint32_t(m_px, tmp.second);
	}
	void mem_stream_write::push_double(const double value)
	{
		CLLIO_ASSERT(m_px != nullptr && (m_px + sizeof(uint64_t)) <= m_px_end);
		UnionCast<double, uint64_t> tmp;
		tmp.first = value;
		_write_bynary_uint64_t(m_px, tmp.second);
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------

	bool mem_stream_write::trypush_int8_t(const int8_t value)
	{
		if(_can_write<uint8_t>())
		{
			UnionCast<int8_t, uint8_t> tmp;
			tmp.first = value;
			*m_px++ = tmp.second;
			return true;
		}
		return false;

	}
	bool mem_stream_write::trypush_int16_t(const int16_t value)
	{
		if(_can_write<uint16_t>())
		{
			UnionCast<int16_t, uint16_t> tmp;
			tmp.first = value;
			_write_bynary_uint16_t(m_px, tmp.second);
			return true;
		}
		return false;

	}
	bool mem_stream_write::trypush_int32_t(const int32_t value)
	{
		if(_can_write<uint32_t>())
		{
			UnionCast<int32_t, uint32_t> tmp;
			tmp.first = value;
			_write_bynary_uint32_t(m_px, tmp.second);
			return true;
		}
		return false;

	}
	bool mem_stream_write::trypush_int64_t(const int64_t value)
	{
		if(_can_write<uint64_t>())
		{
			UnionCast<int64_t, uint64_t> tmp;
			tmp.first = value;
			_write_bynary_uint64_t(m_px, tmp.second);
			return true;
		}
		return false;

	}

	bool mem_stream_write::trypush_uint8_t(const uint8_t value)
	{
		if(_can_write<uint8_t>())
		{
			*m_px++ = value;
			return true;
		}
		return false;
	}
	bool mem_stream_write::trypush_uint16_t(const uint16_t value)
	{
		if(_can_write<uint16_t>())
		{
			_write_bynary_uint16_t(m_px, value);
			return true;
		}
		return false;
	}
	bool mem_stream_write::trypush_uint32_t(const uint32_t value)
	{
		if(_can_write<uint32_t>())
		{
			_write_bynary_uint32_t(m_px, value);
			return true;
		}
		return false;
	}
	bool mem_stream_write::trypush_uint64_t(const uint64_t value)
	{
		if(_can_write<uint64_t>())
		{
			_write_bynary_uint64_t(m_px, value);
			return true;
		}
		return false;
	}

	bool mem_stream_write::trypush_float(const float value)
	{
		if(_can_write<uint32_t>())
		{
			UnionCast<float, uint32_t> tmp;
			tmp.first = value;
			_write_bynary_uint32_t(m_px, tmp.second);
			return true;
		}
		return false;
	}
	bool mem_stream_write::trypush_double(const double value)
	{
		if(_can_write<uint64_t>())
		{
			UnionCast<double, uint64_t> tmp;
			tmp.first = value;
			_write_bynary_uint64_t(m_px, tmp.second);
			return true;
		}
		return false;
	}
}
