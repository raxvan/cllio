
#include <socket.h>

#ifdef CLLIO_SOCKET_WIN32
	#include "socket_wsl2.h"
#endif

#ifdef CLLIO_SOCKET_POSIX
	#include "socket_posix.h"
#endif

namespace cllio
{

#ifdef CLLIO_SOCKET_WIN32
	uint32_t socket_platform_impl::wsa_share{ 0 };
	std::atomic<bool> socket_platform_impl::wsa_lock{ false };
	template <bool ADD_OR_REMOVE>
	inline void _sock_sync()
	{
		while (true)
		{
			if (socket_platform_impl::wsa_lock.exchange(true, std::memory_order_acquire) == false)
				break;

			while (socket_platform_impl::wsa_lock.load(std::memory_order_relaxed))
			{
			}
		}

		if constexpr (ADD_OR_REMOVE)
		{
			uint32_t s = socket_platform_impl::wsa_share++;
			if (s == 0)
			{
				WSADATA init;
				if (WSAStartup(MAKEWORD(2, 2), &init) != 0)
				{
					std::cerr << "WSAStartup(2,2) failed [" << WSAGetLastError() << "], crash imminent!" << std::endl;
					CLLIO_ASSERT_FALSE("wsa failed to init");
				}
			}
		}
		else
		{
			uint32_t s = socket_platform_impl::wsa_share--;
			if (s == 1)
			{
				WSACleanup();
			}
		}

		socket_platform_impl::wsa_lock.store(false, std::memory_order_release);
	}
#else
	template <bool ADD_OR_REMOVE>
	inline void _sock_sync() {}
#endif

	socket_platform_impl::socket_platform_impl()
	{
		auto& t = socket_handle_impl::get(*this);
		t.construct();
	}
	

	tcpsocket::~tcpsocket()
	{
		auto& t = socket_handle_impl::get(*this);
		t.try_destroy();
		_sock_sync<false>();
	}

	tcpsocket::tcpsocket()
	{
		_sock_sync<true>();
	}
	tcpsocket::tcpsocket(tcpsocket&& other) noexcept
	{
		_sock_sync<true>();
		swap(other);
	}
	tcpsocket& tcpsocket::operator=(tcpsocket&& other) noexcept
	{
		tcpsocket tmp;
		swap(tmp);
		swap(other);
		return (*this);
	}
	void tcpsocket::swap(tcpsocket& other)
	{
		auto& t = socket_handle_impl::get(*this);
		auto& o = socket_handle_impl::get(other);
		t.swap(o);
	}
	bool tcpsocket::raw_read(void* buffer, const std::size_t max_size)
	{
		auto& t = socket_handle_impl::get(*this);
		return t.raw_recv_buffered(buffer, max_size);
	}
	bool tcpsocket::raw_write(const void* buffer, const std::size_t size)
	{
		auto& t = socket_handle_impl::get(*this);
		return t.raw_send_buffered(buffer, size);
	}
	bool tcpsocket::connect(const char* ip, const char* port)
	{
		socket_handle_impl tmp;
		if (tmp.connect_to(ip, port))
		{
			tmp.swap(socket_handle_impl::get(*this));
			return true;
		}
		return false;
	}
	bool tcpsocket::accept(const char* port, const std::size_t max_queue)
	{
		socket_handle_impl tmp;
		if (tmp.accept_on(port, max_queue))
		{
			tmp.swap(socket_handle_impl::get(*this));
			return true;
		}
		return false;
	}
	tcpsocket tcpsocket::wait_for_connection()
	{
		tcpsocket result;
		auto& t = socket_handle_impl::get(*this);
		t.wait_for_connection(socket_handle_impl::get(result));
		return result;
	}
}

