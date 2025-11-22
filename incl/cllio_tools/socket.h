
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
		bool pop_uint8(uint8_t& out);
		bool pop_uint16(uint16_t& out);
		bool pop_uint32(uint32_t& out);
		bool pop_uint64(uint64_t& out);

		bool pop_int8(int8_t& out);
		bool pop_int16(int16_t& out);
		bool pop_int32(int32_t& out);
		bool pop_int64(int64_t& out);

		bool pop_float(float& out);
		bool pop_double(double& out);

	public:
		bool try_push_int8(const int8_t value);
		bool try_push_int16(const int16_t value);
		bool try_push_int32(const int32_t value);
		bool try_push_int64(const int64_t value);

		bool try_push_uint8(const uint8_t value);
		bool try_push_uint16(const uint16_t value);
		bool try_push_uint32(const uint32_t value);
		bool try_push_uint64(const uint64_t value);

		bool try_push_float(const float value);
		bool try_push_double(const double value);

	public:
		inline bool write_with_header(const uint64_t header, const void* buffer, const std::size_t size)
		{
			if (try_push_uint64(header))
				return trywrite_raw_buffer(buffer, size);
			return false;
		}

		template <class F>
		// std::pair<void* buffer, std::size_t> T(uint64_t header);
		inline bool read_with_header(const F& _func)
		{
			uint64_t header;
			if (pop_uint64(header))
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
