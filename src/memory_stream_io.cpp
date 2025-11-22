

#include "memory_stream_io.h"
#include "cllio_internal.h"

namespace cllio
{

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	memory_rstream_unchecked::memory_rstream_unchecked(const void* px)
		: m_px(static_cast<const byte_t*>(px))
	{
	}

	//-----------------------------------------------------------------------------------------------------------

	uint8_t memory_rstream_unchecked::pop_uint8()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return *m_px++;
	}
	uint16_t memory_rstream_unchecked::pop_uint16()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_uint16_t(m_px);
	}
	uint32_t memory_rstream_unchecked::pop_uint32()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_uint32_t(m_px);
	}
	uint64_t memory_rstream_unchecked::pop_uint64()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_uint64_t(m_px);
	}

	int8_t memory_rstream_unchecked::pop_int8()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _serializer_utils::_uts8(*m_px++);
	}
	int16_t memory_rstream_unchecked::pop_int16()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_int16_t(m_px);
	}
	int32_t memory_rstream_unchecked::pop_int32()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_int32_t(m_px);
	}
	int64_t memory_rstream_unchecked::pop_int64()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_int64_t(m_px);
	}

	float memory_rstream_unchecked::pop_float()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_float(m_px);
	}
	double memory_rstream_unchecked::pop_double()
	{
		CLLIO_ASSERT(m_px != nullptr);
		return _inline_serializer_utils::_read_double(m_px);
	}

	//-----------------------------------------------------------------------------------------------------------

	memory_rstream::memory_rstream(const void* px, const std::size_t size)
		: m_px(static_cast<const uint8_t*>(px))
		, m_px_end(static_cast<const uint8_t*>(px) + size)
	{
	}
	std::ptrdiff_t memory_rstream::size() const
	{
		CLLIO_ASSERT(m_px <= m_px_end);
		return m_px_end - m_px;
	}

	//-----------------------------------------------------------------------------------------------------------

	void memory_rstream::read_raw_buffer(void* dest, const std::size_t ammount)
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
	bool memory_rstream::tryread_raw_buffer(void* dest, const std::size_t ammount)
	{
		if (m_px != nullptr && (m_px + ammount) <= m_px_end)
		{
			read_raw_buffer(dest, ammount);
			return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------------------------------------------

	bool memory_rstream::pop_uint8(uint8_t& out)
	{
		if (_can_read<uint8_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = *m_px++;
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_uint16(uint16_t& out)
	{
		if (_can_read<uint16_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_uint16_t(m_px);
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_uint32(uint32_t& out)
	{
		if (_can_read<uint32_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_uint32_t(m_px);
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_uint64(uint64_t& out)
	{
		if (_can_read<uint64_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_uint64_t(m_px);
			return true;
		}
		return false;
	}

	bool memory_rstream::pop_int8(int8_t& out)
	{
		if (_can_read<uint8_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _serializer_utils::_uts8(*m_px++);
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_int16(int16_t& out)
	{
		if (_can_read<uint16_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_int16_t(m_px);
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_int32(int32_t& out)
	{
		if (_can_read<uint32_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_int32_t(m_px);
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_int64(int64_t& out)
	{
		if (_can_read<uint64_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_int64_t(m_px);
			return true;
		}
		return false;
	}

	bool memory_rstream::pop_float(float& out)
	{
		if (_can_read<uint32_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_float(m_px);
			return true;
		}
		return false;
	}
	bool memory_rstream::pop_double(double& out)
	{
		if (_can_read<uint64_t>())
		{
			CLLIO_ASSERT(m_px != nullptr);
			out = _inline_serializer_utils::_read_double(m_px);
			return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------------------------------------------

	uint8_t memory_rstream::pop_uint8()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint8_t>());
		return *m_px++;
	}
	uint16_t memory_rstream::pop_uint16()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint16_t>());
		return _inline_serializer_utils::_read_uint16_t(m_px);
	}
	uint32_t memory_rstream::pop_uint32()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint32_t>());
		return _inline_serializer_utils::_read_uint32_t(m_px);
	}
	uint64_t memory_rstream::pop_uint64()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint64_t>());
		return _inline_serializer_utils::_read_uint64_t(m_px);
	}

	int8_t memory_rstream::pop_int8()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint8_t>());
		return _serializer_utils::_uts8(*m_px++);
	}
	int16_t memory_rstream::pop_int16()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint16_t>());
		return _inline_serializer_utils::_read_int16_t(m_px);
	}
	int32_t memory_rstream::pop_int32()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint32_t>());
		return _inline_serializer_utils::_read_int32_t(m_px);
	}
	int64_t memory_rstream::pop_int64()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint64_t>());
		return _inline_serializer_utils::_read_int64_t(m_px);
	}

	float memory_rstream::pop_float()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint32_t>());
		return _inline_serializer_utils::_read_float(m_px);
	}
	double memory_rstream::pop_double()
	{
		CLLIO_ASSERT(m_px != nullptr && _can_read<uint64_t>());
		return _inline_serializer_utils::_read_double(m_px);
	}

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	memory_wstream_iterator::memory_wstream_iterator(void* px)
		: m_px(static_cast<uint8_t*>(px))
	{
	}
	//-----------------------------------------------------------------------------------------------------------

	void memory_wstream_unchecked::push_int8(const int8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		*m_px++ = _serializer_utils::_stu8(value);
	}
	void memory_wstream_unchecked::push_int16(const int16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint16_t(m_px, _serializer_utils::_stu16(value));
	}
	void memory_wstream_unchecked::push_int32(const int32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint32_t(m_px, _serializer_utils::_stu32(value));
	}
	void memory_wstream_unchecked::push_int64(const int64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_stu64(value));
	}

	void memory_wstream_unchecked::push_uint8(const uint8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		*m_px++ = value;
	}
	void memory_wstream_unchecked::push_uint16(const uint16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint16_t(m_px, value);
	}
	void memory_wstream_unchecked::push_uint32(const uint32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint32_t(m_px, value);
	}
	void memory_wstream_unchecked::push_uint64(const uint64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint64_t(m_px, value);
	}

	void memory_wstream_unchecked::push_float(const float value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint32_t(m_px, _serializer_utils::_ftu(value));
	}
	void memory_wstream_unchecked::push_double(const double value)
	{
		CLLIO_ASSERT(m_px != nullptr);
		_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_dtu(value));
	}

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	memory_wstream::memory_wstream(void* px, const std::size_t size_in_bytes)
		: memory_wstream_iterator(px)
		, m_px_end(static_cast<byte_t*>(px) + size_in_bytes)
	{
	}

	void memory_wstream::push_int8(const int8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint8_t>());
		*m_px++ = _serializer_utils::_stu8(value);
	}
	void memory_wstream::push_int16(const int16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint16_t>());
		_inline_serializer_utils::_write_binary_uint16_t(m_px, _serializer_utils::_stu16(value));
	}
	void memory_wstream::push_int32(const int32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint32_t>());
		_inline_serializer_utils::_write_binary_uint32_t(m_px, _serializer_utils::_stu32(value));
	}
	void memory_wstream::push_int64(const int64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint64_t>());
		_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_stu64(value));
	}

	void memory_wstream::push_uint8(const uint8_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint8_t>());
		*m_px++ = value;
	}
	void memory_wstream::push_uint16(const uint16_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint16_t>());
		_inline_serializer_utils::_write_binary_uint16_t(m_px, value);
	}
	void memory_wstream::push_uint32(const uint32_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint32_t>());
		_inline_serializer_utils::_write_binary_uint32_t(m_px, value);
	}
	void memory_wstream::push_uint64(const uint64_t value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint64_t>());
		_inline_serializer_utils::_write_binary_uint64_t(m_px, value);
	}

	void memory_wstream::push_float(const float value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint32_t>());
		_inline_serializer_utils::_write_binary_uint32_t(m_px, _serializer_utils::_ftu(value));
	}
	void memory_wstream::push_double(const double value)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint64_t>());
		_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_dtu(value));
	}
	void memory_wstream::push_ptr(const void* px)
	{
		CLLIO_ASSERT(m_px != nullptr && _can_write<uint64_t>());
		_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_ptu(px));
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------

	bool memory_wstream::try_push_int8(const int8_t value)
	{
		if (_can_write<uint8_t>())
		{
			*m_px++ = _serializer_utils::_stu8(value);
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_int16(const int16_t value)
	{
		if (_can_write<uint16_t>())
		{
			_inline_serializer_utils::_write_binary_uint16_t(m_px, _serializer_utils::_stu16(value));
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_int32(const int32_t value)
	{
		if (_can_write<uint32_t>())
		{
			_serializer_utils::_write_binary_uint32_t(m_px, _serializer_utils::_stu32(value));
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_int64(const int64_t value)
	{
		if (_can_write<uint64_t>())
		{
			_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_stu64(value));
			return true;
		}
		return false;
	}

	bool memory_wstream::try_push_uint8(const uint8_t value)
	{
		if (_can_write<uint8_t>())
		{
			*m_px++ = value;
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_uint16(const uint16_t value)
	{
		if (_can_write<uint16_t>())
		{
			_inline_serializer_utils::_write_binary_uint16_t(m_px, value);
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_uint32(const uint32_t value)
	{
		if (_can_write<uint32_t>())
		{
			_inline_serializer_utils::_write_binary_uint32_t(m_px, value);
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_uint64(const uint64_t value)
	{
		if (_can_write<uint64_t>())
		{
			_inline_serializer_utils::_write_binary_uint64_t(m_px, value);
			return true;
		}
		return false;
	}

	bool memory_wstream::try_push_float(const float value)
	{
		if (_can_write<uint32_t>())
		{
			_inline_serializer_utils::_write_binary_uint32_t(m_px, _serializer_utils::_ftu(value));
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_double(const double value)
	{
		if (_can_write<uint64_t>())
		{
			_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_dtu(value));
			return true;
		}
		return false;
	}
	bool memory_wstream::try_push_ptr(const void* px)
	{
		if (_can_write<uint64_t>())
		{
			_inline_serializer_utils::_write_binary_uint64_t(m_px, _serializer_utils::_ptu(px));
			return true;
		}
		return false;
	}
	bool memory_wstream::trywrite_raw_buffer(const void* data, const std::size_t byte_count)
	{
		if ((m_px != nullptr && (m_px + byte_count) <= m_px_end))
		{
			std::memcpy(m_px, data, byte_count);
			m_px += byte_count;
			return true;
		}
		return false;
	}
	void memory_wstream::write_raw_buffer(const void* data, const std::size_t byte_count)
	{
		CLLIO_ASSERT(((m_px != nullptr && (m_px + byte_count) <= m_px_end)));
		std::memcpy(m_px, data, byte_count);
		m_px += byte_count;
	}

}
