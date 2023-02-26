
#pragma once

#include "cllio_internal_utils.h"

#ifdef CLLIO_SOCKET_IMPL

#	include <array>

#	ifdef _WIN32
#		define CLLIO_SOCKET_WIN32
#	else
#		define CLLIO_SOCKET_POSIX
#	endif

namespace cllio
{

	struct socket_platform_impl
	{
		friend struct socket_handle_impl;

#	ifdef CLLIO_SOCKET_WIN32
		enum
		{
			kDataSize = sizeof(void*)
		};
#	endif

#	ifdef CLLIO_SOCKET_POSIX
		enum
		{
			kDataSize = sizeof(int)
		};
#	endif

		std::array<byte_t, kDataSize> m_handle_data = {};

		socket_platform_impl();
		~socket_platform_impl();
	};

	struct tcpsocket : protected socket_platform_impl
	{
	public:
		tcpsocket(const tcpsocket&) = delete;
		tcpsocket& operator=(const tcpsocket&) = delete;

	public:
		tcpsocket() = default;
		~tcpsocket() = default;

		tcpsocket(tcpsocket&&) noexcept;
		tcpsocket& operator=(tcpsocket&&) noexcept;

	public:
		void swap(tcpsocket& other);

		bool connect(const char* ip, const char* port);
		bool accept(const char* port, const std::size_t max_queue);

		bool wait_for_read(const uint32_t time_ms); //returns true if there is data to read
		tcpsocket wait_for_connection(); // in combination with accept()

		void close();

		bool connected() const;

	public:
		bool tryread_raw_buffer(void* buffer, const std::size_t max_size);
		bool trywrite_raw_buffer(const void* buffer, const std::size_t size);

	public:
		bool pop_uint8_t(uint8_t& out);
		bool pop_uint16_t(uint16_t& out);
		bool pop_uint32_t(uint32_t& out);
		bool pop_uint64_t(uint64_t& out);

		bool pop_int8_t(int8_t& out);
		bool pop_int16_t(int16_t& out);
		bool pop_int32_t(int32_t& out);
		bool pop_int64_t(int64_t& out);

		bool pop_float(float& out);
		bool pop_double(double& out);

	public:
		bool trypush_int8_t(const int8_t value);
		bool trypush_int16_t(const int16_t value);
		bool trypush_int32_t(const int32_t value);
		bool trypush_int64_t(const int64_t value);

		bool trypush_uint8_t(const uint8_t value);
		bool trypush_uint16_t(const uint16_t value);
		bool trypush_uint32_t(const uint32_t value);
		bool trypush_uint64_t(const uint64_t value);

		bool trypush_float(const float value);
		bool trypush_double(const double value);

	public:
		inline bool write_with_header(const uint64_t header, const void* buffer, const std::size_t size)
		{
			if (trypush_uint64_t(header))
				return trywrite_raw_buffer(buffer, size);
			return false;
		}

		template <class F>
		// std::pair<void* buffer, std::size_t> T(uint64_t header);
		inline bool read_with_header(const F& _func)
		{
			uint64_t header;
			if (pop_uint64_t(header))
			{
				auto tr = _func(header);
				if (tr.first)
					return tryread_raw_buffer(tr.first, tr.second);
			}
			return false;
		}
	};

}

#endif
