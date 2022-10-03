#pragma once

#include "../cllio_internal_utils.h"

namespace cllio
{

	struct pack
	{
	public:
		template <class W>
		// 61 bits max precision
		static inline bool write_packed_uint64_t(W& writer, const uint64_t value);

		template <class W>
		// 61 bits max precision
		static inline bool write_packed_int64_t(W& writer, const int64_t value);

		template <class W>
		// 7 bytes max space
		static inline bool write_packed_size(W& writer, const std::size_t v);


	public:
		template <class R>
		// 61 bits max precision
		static inline bool read_packed_uint64_t(R& reader, uint64_t& value);

		template <class R>
		// 61 bits max precision
		static inline bool read_packed_int64_t(R& reader, int64_t& value);

	public:

		//std::size_t readers

		template <class R>
		// 7 bytes max space
		static inline bool read_packed_size(R& reader, std::size_t& out);

		template <class R, class F>
		// void F(std::size_t)
		static inline bool read_packed_size_callback(R& reader, const F& _callback);


	};



	template <class W>
	// 61 bits max precision
	inline bool pack::write_packed_uint64_t(W& writer, const uint64_t value)
	{
		std::array<byte_t, 8> buffer;
		std::size_t i = _serializer_utils::_serialize_packed_uint64_t(buffer, value);

		return i < buffer.size() && writer.trywrite_raw_buffer(&buffer[i], buffer.size() - i);
	}
	template <class W>
	// 61 bits max precision
	inline bool pack::write_packed_int64_t(W& writer, const int64_t value)
	{
		uint64_t v;
		if (value < 0)
			v = uint64_t(-value) << 1 | uint64_t(1);
		else
			v = uint64_t(value) << 1;
		return write_packed_uint64_t(writer, v);
	}
	template <class W>
	// 7 bytes max space
	inline bool pack::write_packed_size(W& writer, const std::size_t v)
	{
		return write_packed_uint64_t(writer, v);
	}


	template <class R>
	// 61 bits max precision
	inline bool pack::read_packed_uint64_t(R& reader, uint64_t& value)
	{
		std::array<byte_t,8> buffer;
		if (reader.pop_uint8_t(buffer[0]) == false)
			return false;

		uint8_t sz = buffer[0] >> 5;
		if(sz > 0)
		{
			if (reader.tryread_raw_buffer(&buffer[1], sz) == false)
				return false;
		}

		value = _serializer_utils::_deserialize_packed_uint64_t(buffer, sz);
		return true;
	}
	template <class R>
	// 61 bits max precision
	inline bool pack::read_packed_int64_t(R& reader, int64_t& value)
	{
		uint64_t v;
		if (read_packed_uint64_t(reader, v) == false)
			return false;

		value = -int64_t(v >> 1);
		if ((value & 1) != 0)
			value = -value;
		return true;
	}
	template <class R>
	// 7 bytes max space
	inline bool pack::read_packed_size(R& reader, std::size_t& out)
	{
		uint64_t tmp;
		if (read_packed_uint64_t(reader, tmp) == false)
			return false;
		CLLIO_ASSERT(tmp < std::numeric_limits<std::size_t>::max());
		out = std::size_t(tmp);
		return true;
	}
	template <class R, class F>
	// void F(std::size_t)
	inline bool pack::read_packed_size_callback(R& reader, const F& _callback)
	{
		uint64_t tmp;
		if (read_packed_uint64_t(reader, tmp) == false)
			return false;
		CLLIO_ASSERT(tmp < std::numeric_limits<std::size_t>::max());
		return _callback(std::size_t(tmp));
	}

}
