
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN 1
#endif
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#ifdef max
	#undef max
#endif

namespace cllio
{
	struct socket_handle_impl
	{
		SOCKET sock;
		// see https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code
		// see https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

		inline void construct()
		{
			sock = INVALID_SOCKET;
		}

		inline void try_destroy()
		{
			if (sock != INVALID_SOCKET)
				closesocket(sock);
		}

		inline bool connect_to(const char * ip, const char * port)
		{
			CLLIO_ASSERT(sock == INVALID_SOCKET);
			struct addrinfo hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			struct addrinfo* result = nullptr;
			int iResult = getaddrinfo(ip, port, &hints, &result);
			if (iResult != 0)
				return false;

			SOCKET s = INVALID_SOCKET;
			for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				// Create a SOCKET for connecting to server
				s = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (s == INVALID_SOCKET)
				{
					freeaddrinfo(result);
					return false;
				}

				// Connect to server.
				iResult = ::connect(s, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (iResult == SOCKET_ERROR) {
					closesocket(s);
					s = INVALID_SOCKET;
					continue;
				}
				break;
			}
			freeaddrinfo(result);

			if (s == INVALID_SOCKET)
				return false;

			sock = s;

			return true;
		}
		inline bool accept_on(const char* port, const std::size_t max_queue)
		{
			CLLIO_ASSERT(sock == INVALID_SOCKET);
			struct addrinfo hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			struct addrinfo* result = nullptr;
			int iResult = getaddrinfo(NULL, port, &hints, &result);
			if (iResult != 0)
				return false;

			SOCKET ls = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (ls == INVALID_SOCKET)
			{
				freeaddrinfo(result);
				return false;
			}
			iResult = ::bind(ls, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				freeaddrinfo(result);
				closesocket(ls);
				return false;
			}

			freeaddrinfo(result);

			iResult = ::listen(ls, int(max_queue) );
			if (iResult == SOCKET_ERROR)
			{
				closesocket(ls);
				return false;
			}

			sock = ls;

			return true;
		}

		inline bool wait_for_connection(socket_handle_impl& out) const
		{
			CLLIO_ASSERT(sock != INVALID_SOCKET);
			auto s = ::accept(sock, NULL, NULL);
			if (s == INVALID_SOCKET)
			{
				closesocket(sock);
				return false;
			}
			out.sock = s;
			return true;
		}

		inline bool raw_send_buffered(const void* buffer, const std::size_t size)
		{
			CLLIO_ASSERT(sock != INVALID_SOCKET);
			const char* itr = (const char*)buffer;
			const char* end = itr + size;
			do
			{
				int iResult = ::send(sock, (const char*)buffer, (int)size, 0);
				if (iResult == SOCKET_ERROR)
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return false;
				}
				CLLIO_ASSERT(iResult > 0);
				itr += iResult;
			} while (itr < end);
			return true;
		}

		inline bool raw_recv_buffered(void* buffer, const std::size_t max_size)
		{
			CLLIO_ASSERT(sock != INVALID_SOCKET);
			const char* itr = (const char*)buffer;
			const char* end = itr + max_size;
			do
			{
				CLLIO_ASSERT(max_size < std::size_t(std::numeric_limits<int32_t>::max()));
				int iResult = ::recv(sock, (char*)(buffer), int(max_size), 0);
				if (iResult == SOCKET_ERROR)
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return false;
				}
				CLLIO_ASSERT(iResult > 0);
				itr += iResult;
			} while (itr < end);
			return true;
		}
		inline void swap(socket_handle_impl& other)
		{
			std::swap(sock, other.sock);
		}
		inline bool valid() const
		{
			return sock != INVALID_SOCKET;
		}

		static inline socket_handle_impl& get(socket_platform_impl& pi)
		{
			static_assert(sizeof(socket_handle_impl) <= socket_platform_impl::kDataSize, "Invalid handle");
			return *static_cast<socket_handle_impl*>(static_cast<void*>(pi.m_handle_data.data()));
		}
		static inline const socket_handle_impl& get(const socket_platform_impl& pi)
		{
			static_assert(sizeof(socket_handle_impl) <= socket_platform_impl::kDataSize, "Invalid handle");
			return *static_cast<const socket_handle_impl*>(static_cast<const void*>(pi.m_handle_data.data()));
		}
	};
}