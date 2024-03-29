#pragma once

#include "../cllio_internal_utils.h"

namespace cllio
{
	// can measure sized used by writers

	struct write_size
	{
	protected:
		std::size_t m_size = 0;

	public:
		inline std::size_t size() const
		{
			return m_size;
		}

	protected:
		template <class T>
		inline void _add_size()
		{
			m_size += sizeof(T);
		}

		inline void _add_size(const std::size_t sz)
		{
			m_size += sz;
		}

	public:
		inline void write_raw_buffer(const void*, const std::size_t byte_count)
		{
			_add_size(byte_count);
		}
		inline bool trywrite_raw_buffer(const void*, const std::size_t byte_count)
		{
			_add_size(byte_count);
			return true;
		}

	public:
		inline void push_int8_t(const int8_t)
		{
			_add_size<int8_t>();
		}
		inline void push_int16_t(const int16_t)
		{
			_add_size<int16_t>();
		}
		inline void push_int32_t(const int32_t)
		{
			_add_size<int32_t>();
		}
		inline void push_int64_t(const int64_t)
		{
			_add_size<int64_t>();
		}

		inline void push_uint8_t(const uint8_t)
		{
			_add_size<uint8_t>();
		}
		inline void push_uint16_t(const uint16_t)
		{
			_add_size<uint16_t>();
		}
		inline void push_uint32_t(const uint32_t)
		{
			_add_size<uint32_t>();
		}
		inline void push_uint64_t(const uint64_t)
		{
			_add_size<uint64_t>();
		}

		inline void push_float(const float)
		{
			_add_size<uint32_t>();
		}
		inline void push_double(const double)
		{
			_add_size<uint64_t>();
		}
		inline void push_ptr(const void*)
		{
			_add_size<uint64_t>();
		}

	public:
		inline void trypush_int8_t(const int8_t)
		{
			_add_size<int8_t>();
		}
		inline void trypush_int16_t(const int16_t)
		{
			_add_size<int16_t>();
		}
		inline void trypush_int32_t(const int32_t)
		{
			_add_size<int32_t>();
		}
		inline void trypush_int64_t(const int64_t)
		{
			_add_size<int64_t>();
		}

		inline void trypush_uint8_t(const uint8_t)
		{
			_add_size<uint8_t>();
		}
		inline void trypush_uint16_t(const uint16_t)
		{
			_add_size<uint16_t>();
		}
		inline void trypush_uint32_t(const uint32_t)
		{
			_add_size<uint32_t>();
		}
		inline void trypush_uint64_t(const uint64_t)
		{
			_add_size<uint64_t>();
		}

		inline void trypush_float(const float)
		{
			_add_size<uint32_t>();
		}
		inline void trypush_double(const double)
		{
			_add_size<uint64_t>();
		}
		inline void trypush_ptr(const void*)
		{
			_add_size<uint64_t>();
		}
	};
}
