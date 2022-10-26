
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
	bool tcpsocket::tryread_raw_buffer(void* buffer, const std::size_t max_size)
	{
		auto& t = socket_handle_impl::get(*this);
		return t.raw_recv_buffered(buffer, max_size);
	}
	bool tcpsocket::trywrite_raw_buffer(const void* buffer, const std::size_t size)
	{
		auto& t = socket_handle_impl::get(*this);
		return t.raw_send_buffered(buffer, size);
	}

	bool tcpsocket::connect(const char* ip, const char* port)
	{
		socket_handle_impl tmp;
		tmp.construct();
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
		tmp.construct();
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
	bool tcpsocket::connected() const
	{
		const auto& t = socket_handle_impl::get(*this);
		return t.valid();
	}
	void tcpsocket::close()
	{
		auto& t = socket_handle_impl::get(*this);
		return t.close();
	}

	bool tcpsocket::pop_uint8_t(uint8_t& out)
	{
		byte_t buffer;
		if(tryread_raw_buffer(&buffer,sizeof(uint8_t) * 1))
		{
			out = buffer;
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_uint16_t(uint16_t& out)
	{
		byte_t buffer[sizeof(uint16_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint16_t)))
		{
			out = _serializer_utils::_read_uint16_t(&buffer[0]);
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_uint32_t(uint32_t& out)
	{
		byte_t buffer[sizeof(uint32_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint32_t)))
		{
			out = _serializer_utils::_read_uint32_t(&buffer[0]);
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_uint64_t(uint64_t& out)
	{
		byte_t buffer[sizeof(uint64_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint64_t)))
		{
			out = _serializer_utils::_read_uint64_t(&buffer[0]);
			return true;
		}
		return false;
	}

	bool tcpsocket::pop_int8_t(int8_t& out){
		byte_t buffer;
		if(tryread_raw_buffer(&buffer,sizeof(uint8_t) * 1))
		{
			out = _serializer_utils::_uts8(buffer);
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_int16_t(int16_t& out)
	{
		byte_t buffer[sizeof(uint16_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint16_t)))
		{
			out = _serializer_utils::_uts16(_serializer_utils::_read_uint16_t(&buffer[0]));
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_int32_t(int32_t& out)
	{
		byte_t buffer[sizeof(uint32_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint32_t)))
		{
			out = _serializer_utils::_uts32(_serializer_utils::_read_uint32_t(&buffer[0]));
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_int64_t(int64_t& out)
	{
		byte_t buffer[sizeof(uint64_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint64_t)))
		{
			out = _serializer_utils::_uts64(_serializer_utils::_read_uint64_t(&buffer[0]));
			return true;
		}
		return false;
	}

	bool tcpsocket::pop_float(float& out)
	{
		byte_t buffer[sizeof(uint32_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint32_t)))
		{
			out = _serializer_utils::_utf(_serializer_utils::_read_uint32_t(&buffer[0]));
			return true;
		}
		return false;
	}
	bool tcpsocket::pop_double(double& out)
	{
		byte_t buffer[sizeof(uint64_t)];
		if(tryread_raw_buffer(&buffer[0],sizeof(uint64_t)))
		{
			out = _serializer_utils::_utd(_serializer_utils::_read_uint64_t(&buffer[0]));
			return true;
		}
		return false;
	}

	
	bool tcpsocket::trypush_int8_t(const int8_t value)
	{
		byte_t buffer;
		buffer = _serializer_utils::_stu8(value);
		return trywrite_raw_buffer(&buffer,sizeof(uint8_t));
	}
	bool tcpsocket::trypush_int16_t(const int16_t value)
	{
		byte_t buffer[sizeof(uint16_t)];
		_serializer_utils::_write_bynary_uint16_t(&buffer[0],_serializer_utils::_stu16(value));
		return trywrite_raw_buffer(&buffer,sizeof(uint16_t));
	}
	bool tcpsocket::trypush_int32_t(const int32_t value)
	{
		byte_t buffer[sizeof(uint32_t)];
		_serializer_utils::_write_bynary_uint32_t(&buffer[0],_serializer_utils::_stu32(value));
		return trywrite_raw_buffer(&buffer,sizeof(uint32_t));
	}
	bool tcpsocket::trypush_int64_t(const int64_t value)
	{
		byte_t buffer[sizeof(uint64_t)];
		_serializer_utils::_write_bynary_uint64_t(&buffer[0],_serializer_utils::_stu64(value));
		return trywrite_raw_buffer(&buffer,sizeof(uint64_t));
	}

	bool tcpsocket::trypush_uint8_t(const uint8_t value)
	{
		byte_t buffer;
		buffer = value;
		return trywrite_raw_buffer(&buffer,sizeof(uint8_t));
	}

	bool tcpsocket::trypush_uint16_t(const uint16_t value)
	{
		byte_t buffer[sizeof(uint16_t)];
		_serializer_utils::_write_bynary_uint64_t(&buffer[0],value);
		return trywrite_raw_buffer(&buffer,sizeof(uint16_t));
	}
	bool tcpsocket::trypush_uint32_t(const uint32_t value)
	{
		byte_t buffer[sizeof(uint32_t)];
		_serializer_utils::_write_bynary_uint64_t(&buffer[0],value);
		return trywrite_raw_buffer(&buffer,sizeof(uint32_t));
	}
	bool tcpsocket::trypush_uint64_t(const uint64_t value)
	{
		byte_t buffer[sizeof(uint64_t)];
		_serializer_utils::_write_bynary_uint64_t(&buffer[0],value);
		return trywrite_raw_buffer(&buffer,sizeof(uint64_t));
	}

	bool tcpsocket::trypush_float(const float value)
	{
		byte_t buffer[sizeof(uint32_t)];
		_serializer_utils::_write_bynary_uint32_t(&buffer[0],_serializer_utils::_ftu(value));
		return trywrite_raw_buffer(&buffer,sizeof(uint32_t));
	}
	bool tcpsocket::trypush_double(const double value)
	{
		byte_t buffer[sizeof(uint64_t)];
		_serializer_utils::_write_bynary_uint64_t(&buffer[0],_serializer_utils::_dtu(value));
		return trywrite_raw_buffer(&buffer,sizeof(uint64_t));
	}
}

#endif
