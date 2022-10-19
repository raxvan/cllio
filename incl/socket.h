
#pragma once

#include "cllio_utils.h"
#include <atomic>
#include <array>

#ifndef CLLIO_SOCKET
#	ifdef _WIN32
#		define CLLIO_SOCKET_WIN32
#	else
#		define CLLIO_SOCKET_POSIX
#	endif
#endif

namespace cllio
{

	struct socket_platform_impl
	{
		friend struct socket_handle_impl;

#ifdef CLLIO_SOCKET_WIN32
		enum
		{
			kDataSize = sizeof(void*)
		};
		static std::atomic<bool> wsa_lock;
		static uint32_t			 wsa_share;
#endif

#ifdef CLLIO_SOCKET_POSIX
		enum
		{
			kDataSize = sizeof(int)
		};
#endif

		std::array<byte_t, kDataSize> m_handle_data = {};

		socket_platform_impl();
	};

	struct tcpsocket : protected socket_platform_impl
	{
	public:
		tcpsocket(const tcpsocket&) = delete;
		tcpsocket& operator=(const tcpsocket&) = delete;

	public:
		tcpsocket();
		~tcpsocket();
		tcpsocket(tcpsocket&&) noexcept;
		tcpsocket& operator=(tcpsocket&&) noexcept;

	public:
		void swap(tcpsocket& other);

		bool connect(const char* ip, const char* port);
		bool accept(const char* port, const std::size_t max_queue);

		tcpsocket wait_for_connection();

	public:
		// inline bool connected() const;
	public:
		bool raw_read(void* buffer, const std::size_t max_size);
		bool raw_write(const void* buffer, const std::size_t size);
	};

}
