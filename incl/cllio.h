#pragma once

#include "memory_stream_io.h"
#include "stdfile_stream_io.h"

namespace cllio
{

	struct utils
	{
	public:
		static byte_t fromhex(char first, char second);

		template <class W>
		// 61 bits max precision
		static inline bool write_packed_uint64_t(W& writer, const uint64_t value)
		{
			const uint8_t low_mask = (1 << 5) - 1;

			if (value <= low_mask) // first 5 bits
				return writer.trypush_uint8_t(char(value));

			uint8_t buffer[9];

			std::size_t itr = 0;
			{
				uint64_t tmp = value;
				while (tmp != 0)
				{
					buffer[1 + itr++] = tmp & 255;
					tmp = tmp >> 8;
				}
				if (itr == 8 && buffer[8] > low_mask)
					return false; // can't fit

				buffer[0] = uint8_t(itr << 5);

				if (buffer[itr] <= low_mask)
				{
					buffer[0] = uint8_t((itr - 1) << 5) | buffer[itr];
				}
				else
				{
					buffer[0] = uint8_t(itr << 5);
					itr++;
				}
			}

			return writer.trywrite_raw_buffer(&buffer[0], itr);
		}
		template <class W>
		// 61 bits max precision
		static inline bool write_packed_int64_t(W& writer, const int64_t value)
		{
			uint64_t v;
			if (value < 0)
				v = uint64_t(-value) << 1 | uint64_t(1);
			else
				v = uint64_t(value) << 1;
			return write_packed_uint64_t(writer, v);
		}
		template <class W>
		// 7 bytes max precision
		static inline bool write_packed_size(W& writer, const std::size_t v)
		{
			return write_packed_uint64_t(writer, v);
		}

	public:
		template <class R>
		// 61 bits max precision
		static inline bool read_packed_uint64_t(R& reader, uint64_t& value)
		{
			const uint8_t low_mask = (1 << 5) - 1;
			uint8_t		  p;
			if (reader.pop_uint8_t(p) == false)
				return false;

			if (p <= low_mask)
			{
				value = p;
				return true;
			}

			uint8_t buffer[8];
			uint8_t bytes = (p >> 5) & 7;

			if (reader.tryread_raw_buffer(&buffer[0], bytes) == false)
				return false;

			value = p & low_mask;
			for (uint8_t i = 0; i < bytes; i++)
				value = value << 8 | uint64_t(buffer[bytes - i - 1]);

			return true;
		}
		template <class R>
		// 61 bits max precision
		static inline bool read_packed_int64_t(R& reader, int64_t& value)
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
		// 7 bytes max precision
		static inline bool read_packed_size(R& reader, std::size_t& out)
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
		static inline bool read_packed_size_callback(R& reader, const F& _callback)
		{
			uint64_t tmp;
			if (read_packed_uint64_t(reader, tmp) == false)
				return false;
			CLLIO_ASSERT(tmp < std::numeric_limits<std::size_t>::max());
			return _callback(std::size_t(tmp));
		}
	};

}
