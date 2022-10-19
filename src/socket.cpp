
#include <cllio_tools/socket.h>

#ifdef CLLIO_SOCKET_IMPL

#ifdef CLLIO_SOCKET_WIN32
	#include "socket_wsl2.h"
#endif

#ifdef CLLIO_SOCKET_POSIX
	#include "socket_posix.h"
#endif

namespace cllio
{

	socket_platform_impl::socket_platform_impl()
	{
		auto& t = socket_handle_impl::get(*this);
		t.construct();
	}
	socket_platform_impl::~socket_platform_impl()
	{
		auto& t = socket_handle_impl::get(*this);
		t.try_destroy();
	}

	tcpsocket::tcpsocket(tcpsocket&& other) noexcept
		:tcpsocket()
	{
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

#endif
