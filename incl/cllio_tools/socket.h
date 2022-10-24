
#pragma once

#include "cllio_internal_utils.h"

#ifdef CLLIO_SOCKET_IMPL

#include <array>

#ifdef _WIN32
	#define CLLIO_SOCKET_WIN32
#else
	#define CLLIO_SOCKET_POSIX
#endif

namespace cllio
{

	struct socket_platform_impl
	{
		friend struct socket_handle_impl;

#ifdef CLLIO_SOCKET_WIN32
		enum {
			kDataSize = sizeof(void*)
		};
#endif

#ifdef CLLIO_SOCKET_POSIX
		enum {
			kDataSize = sizeof(int)
		};
#endif

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

		tcpsocket wait_for_connection();

		bool valid() const;
		void close();

	public:
		//inline bool connected() const;
	public:
		bool raw_read(void* buffer, const std::size_t max_size);
		bool raw_write(const void* buffer, const std::size_t size);

	public:
		inline bool write_buffer_with_size(const void* buffer, const std::size_t sz);
		{
			uint64_t wsz = sz;
			if(raw_write(&wsz, sizeof(uint64_t)) == false)
				return false;
			raw_write(buffer,sz);
		}
		template <class F>
		inline bool read_buffer_with_size(const F& _allc_func)
		{
			uint64_t sz = 0;
			if(raw_read(&sz, sizeof(uint64_t)) == false)
				return false;
			void* data = _allc_func(sz);
			if(raw_read(data, sz) == false)
				return false;

			return true;
		}

	};

}

#endif