
#pragma once

#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
namespace cllio
{
	struct socket_handle_impl
	{
		int sock;
		// see https://man7.org/linux/man-pages/man2/socket.2.html

		inline void construct()
		{
			sock = -1;
		}

		inline void try_destroy()
		{
			if (sock != -1)
				close(sock);
		}

		inline bool connect_to(const char* ip, const char* port)
		{
			CLLIO_ASSERT(sock == -1);
			int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (s < 0)
				return false;

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = (unsigned short)atoi(port);
			inet_pton(AF_INET, ip, &(addr.sin_addr.s_addr));

			if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
			{
				close(s);
				return false;
			}
			sock = s;
			return true;
		}
		inline bool accept_on(const char* port, const std::size_t max_queue)
		{
			CLLIO_ASSERT(sock == -1);
			int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (s < 0)
				return false;

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = (unsigned short)atoi(port);
			addr.sin_addr.s_addr = INADDR_ANY;

			if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
			{
				close(s);
				return false;
			}

			if (listen(s, int(max_queue)) < 0)
			{
				close(s);
				return false;
			}

			sock = s;
			return true;
		}

		inline bool wait_for_connection(socket_handle_impl& out) const
		{
			CLLIO_ASSERT(sock != -1);
			struct sockaddr_in addr;
			socklen_t		   len = sizeof(addr);
			auto			   s = ::accept(sock, (struct sockaddr*)&addr, &len);
			if (s == -1)
			{
				close(sock);
				return false;
			}
			out.sock = s;
			return true;
		}

		inline bool wait_for_read(const uint32_t timeout_ms)
		{
			CLLIO_ASSERT(sock != -1);

			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(sock, &fds);

			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = timeout_ms * 1000; //microseconds

			int result = ::select(sock + 1, &fds, NULL, NULL, &timeout);
			if (result == 0)
			{
				return false;
			}
			else if (result == -1)
			{
				//close(sock);
				//sock = -1;
				std::cerr << "Socker error: " << strerror(errno) << std::endl;
				return false;
			}

			return true;
		}
		inline bool raw_send_buffered(const void* buffer, const std::size_t total_size)
		{
			CLLIO_ASSERT(sock != -1);
			const char* itr = (char*)buffer;
			const char* end = itr + total_size;
			int size = int(total_size);
			CLLIO_ASSERT(total_size < std::size_t(std::numeric_limits<int32_t>::max()));
			do
			{
				ssize_t iResult = ::send(sock, itr, size, 0);
				if (iResult <= 0)
				{
					close(sock);
					sock = -1;
					return false;
				}
				CLLIO_ASSERT(iResult > 0);
				itr += iResult;
				size -= iResult;
			} while (itr < end);
			return true;
		}
		inline bool raw_recv_buffered(void* buffer, const std::size_t total_size)
		{
			CLLIO_ASSERT(sock != -1);
			char* itr = (char*)buffer;
			const char* end = itr + total_size;
			int size = int(total_size);
			CLLIO_ASSERT(total_size < std::size_t(std::numeric_limits<int32_t>::max()));
			do
			{
				CLLIO_ASSERT(size > 0);
				ssize_t iResult = ::recv(sock, itr, size, 0);
				if (iResult <= 0)
				{
					close(sock);
					sock = -1;
					return false;
				}
				itr += iResult;
				size -= iResult;
			} while (itr < end);

			return true;
		}

		inline void close_socket()
		{
			if (sock != -1)
			{
				close(sock);
				sock = -1;
			}
		}

		inline void swap(socket_handle_impl& other)
		{
			std::swap(sock, other.sock);
		}
		inline bool valid() const
		{
			return sock != -1;
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
