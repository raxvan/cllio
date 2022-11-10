

#include <stdfile_stream_io.h>
#include "cllio_internal.h"
#include <array>

namespace cllio
{

	std_file_handle::~std_file_handle()
	{
		if (m_file_ptr != nullptr)
			std::fclose(m_file_ptr);
	}
	bool std_file_handle::close()
	{
		if (m_file_ptr != nullptr)
		{
			std::fclose(m_file_ptr);
			m_file_ptr = nullptr;
			return true;
		}
		return false;
	}
	void std_file_handle::swap(std_file_handle& other)
	{
		std::swap(m_file_ptr, other.m_file_ptr);
	}
	std_file_handle::std_file_handle(std::FILE* handle)
		: m_file_ptr(handle)
	{
	}
	bool std_file_handle::isOpen() const
	{
		return m_file_ptr != nullptr;
	}
	std::size_t std_file_handle::get_file_size()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t cursor = std::ftell(m_file_ptr);
		std::fseek(m_file_ptr, 0, SEEK_END);
		std::size_t filesize = std::ftell(m_file_ptr);

		std::fseek(m_file_ptr, (long)cursor, SEEK_SET);
		return filesize;
	}
	std::size_t std_file_handle::get_remaining_size()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t cursor = std::ftell(m_file_ptr);
		std::fseek(m_file_ptr, 0, SEEK_END);
		std::size_t filesize = std::ftell(m_file_ptr);

		std::fseek(m_file_ptr, (long)cursor, SEEK_SET);
		CLLIO_ASSERT(filesize >= cursor);
		return filesize - cursor;
	}
	std::FILE* std_file_handle::get_handle()
	{
		return m_file_ptr;
	}
	//--------------------------------------------------------------------------------------------------------------------
	stdfile_wstream_impl::stdfile_wstream_impl(std::FILE* handle)
		: std_file_handle(handle)
	{
	}
	stdfile_rstream_impl::stdfile_rstream_impl(std::FILE* handle)
		: std_file_handle(handle)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool stdfile_wstream_impl::create(const char* abs_path, const bool binary, const bool append)
	{
		if (m_file_ptr != nullptr)
			std::fclose(m_file_ptr);
#ifdef _MSC_VER
		errno_t err;
		if (append)
		{
			if (binary)
				err = fopen_s(&m_file_ptr, abs_path, "ab");
			else
				err = fopen_s(&m_file_ptr, abs_path, "a");
		}
		else
		{
			if (binary)
				err = fopen_s(&m_file_ptr, abs_path, "wb");
			else
				err = fopen_s(&m_file_ptr, abs_path, "w");
		}
		if (err == 0)
		{
			return true; // ok
		}
		m_file_ptr = nullptr;
		return false;
#else
		if (append)
		{
			if (binary)
				m_file_ptr = std::fopen(abs_path, "ab");
			else
				m_file_ptr = std::fopen(abs_path, "a");
		}
		else
		{
			if (binary)
				m_file_ptr = std::fopen(abs_path, "wb");
			else
				m_file_ptr = std::fopen(abs_path, "w");
		}
		return m_file_ptr != nullptr;
#endif
	}

	void stdfile_wstream_impl::flush()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::fflush(m_file_ptr);
	}

	//--------------------------------------------------------------------------------------------------------------------

	bool stdfile_rstream_impl::open(const char* abs_path, const bool is_binary)
	{
		if (is_binary)
			return open_binary(abs_path);
		return open(abs_path);
	}
	bool stdfile_rstream_impl::open(const char* abs_path)
	{
		if (m_file_ptr != nullptr)
			std::fclose(m_file_ptr);
#ifdef _MSC_VER
		errno_t err = fopen_s(&m_file_ptr, abs_path, "r");
		if (err == 0)
			return true; // ok

		m_file_ptr = nullptr; // TODO: check if this assignment is needed
		return false;
#else
		m_file_ptr = std::fopen(abs_path, "r");
		return m_file_ptr != nullptr;
#endif
	}
	bool stdfile_rstream_impl::open_binary(const char* abs_path)
	{
		if (m_file_ptr != nullptr)
			std::fclose(m_file_ptr);
#ifdef _MSC_VER
		errno_t err = fopen_s(&m_file_ptr, abs_path, "rb");
		if (err == 0)
			return true; // ok

		m_file_ptr = nullptr; // TODO: check if this assignment is needed
		return false;
#else
		m_file_ptr = std::fopen(abs_path, "rb");
		return m_file_ptr != nullptr;
#endif
	}

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------

	void stdfile_wstream_impl::push_int8_t(const int8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		buffer[0] = _serializer_utils::_stu8(value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}
	void stdfile_wstream_impl::push_int16_t(const int16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		_serializer_utils::_write_bynary_uint16_t(buffer.data(), _serializer_utils::_stu16(value));
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}
	void stdfile_wstream_impl::push_int32_t(const int32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		_serializer_utils::_write_bynary_uint32_t(buffer.data(), _serializer_utils::_stu32(value));
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}
	void stdfile_wstream_impl::push_int64_t(const int64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		_serializer_utils::_write_bynary_uint64_t(buffer.data(), _serializer_utils::_stu64(value));
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}

	void stdfile_wstream_impl::push_uint8_t(const uint8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		buffer[0] = value;
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}
	void stdfile_wstream_impl::push_uint16_t(const uint16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		_serializer_utils::_write_bynary_uint16_t(buffer.data(), value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}
	void stdfile_wstream_impl::push_uint32_t(const uint32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		_serializer_utils::_write_bynary_uint32_t(buffer.data(), value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}
	void stdfile_wstream_impl::push_uint64_t(const uint64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		_serializer_utils::_write_bynary_uint64_t(buffer.data(), value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		consider_it_used(sz);
	}

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	bool stdfile_wstream_impl::trypush_int8_t(const int8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		buffer[0] = _serializer_utils::_stu8(value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}
	bool stdfile_wstream_impl::trypush_int16_t(const int16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		_serializer_utils::_write_bynary_uint16_t(buffer.data(), _serializer_utils::_stu16(value));
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}
	bool stdfile_wstream_impl::trypush_int32_t(const int32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		_serializer_utils::_write_bynary_uint32_t(buffer.data(), _serializer_utils::_stu32(value));
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}
	bool stdfile_wstream_impl::trypush_int64_t(const int64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		_serializer_utils::_write_bynary_uint64_t(buffer.data(), _serializer_utils::_stu64(value));
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}

	bool stdfile_wstream_impl::trypush_uint8_t(const uint8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		buffer[0] = value;
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}
	bool stdfile_wstream_impl::trypush_uint16_t(const uint16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		_serializer_utils::_write_bynary_uint16_t(buffer.data(), value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}
	bool stdfile_wstream_impl::trypush_uint32_t(const uint32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		_serializer_utils::_write_bynary_uint32_t(buffer.data(), value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}
	bool stdfile_wstream_impl::trypush_uint64_t(const uint64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		_serializer_utils::_write_bynary_uint64_t(buffer.data(), value);
		std::size_t sz = std::fwrite(buffer.data(), 1, buffer.size(), m_file_ptr);
		return (sz == buffer.size());
	}

	//--------------------------------------------------------------------------------------------------------------------

	void stdfile_wstream_impl::push_float(const float value)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		push_uint32_t(_serializer_utils::_ftu(value));
	}
	void stdfile_wstream_impl::push_double(const double value)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		push_uint64_t(_serializer_utils::_dtu(value));
	}
	void stdfile_wstream_impl::push_ptr(const void* px)
	{
		push_uint64_t(_serializer_utils::_ptu(px));
	}
	bool stdfile_wstream_impl::trypush_float(const float value)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		return trypush_uint32_t(_serializer_utils::_ftu(value));
	}
	bool stdfile_wstream_impl::trypush_double(const double value)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		return trypush_uint64_t(_serializer_utils::_dtu(value));
	}
	bool stdfile_wstream_impl::trypush_ptr(const void* px)
	{
		return trypush_uint64_t(_serializer_utils::_ptu(px));
	}

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------

	void stdfile_wstream_impl::write_raw_buffer(const void* data, const std::size_t byte_count)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(data, 1, byte_count, m_file_ptr);
		CLLIO_ASSERT(sz == byte_count);
		consider_it_used(sz);
	}
	bool stdfile_wstream_impl::trywrite_raw_buffer(const void* data, const std::size_t byte_count)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(data, 1, byte_count, m_file_ptr);
		return (sz == byte_count);
	}

	//--------------------------------------------------------------------------------------------------------------------

	std::size_t stdfile_wstream_impl::str_printf(const char* format, ...)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		va_list args;
		va_start(args, format);
		auto r = std::vfprintf(m_file_ptr, format, args);
		va_end(args);
		if (r < 0)
			r = 0;
		return static_cast<std::size_t>(r);
	}

	std::size_t stdfile_wstream_impl::str(const char* format)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		auto r = std::fprintf(m_file_ptr, "%s", format);
		if (r < 0)
			r = 0;
		return static_cast<std::size_t>(r);
	}
	std::size_t stdfile_wstream_impl::str(const int32_t v)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		auto r = std::fprintf(m_file_ptr, "%i", v);
		if (r < 0)
			r = 0;
		return static_cast<std::size_t>(r);
	}
	std::size_t stdfile_wstream_impl::str(const uint32_t v)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		auto r = std::fprintf(m_file_ptr, "%u", v);
		if (r < 0)
			r = 0;
		return static_cast<std::size_t>(r);
	}

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------------

	void stdfile_rstream_impl::read_raw_buffer(void* dest, const std::size_t ammount)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fread(dest, 1, ammount, m_file_ptr);
		CLLIO_ASSERT(sz == ammount);
		consider_it_used(sz);
	}
	bool stdfile_rstream_impl::tryread_raw_buffer(void* dest, const std::size_t ammount)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fread(dest, 1, ammount, m_file_ptr);
		return (sz == ammount);
	}

	//--------------------------------------------------------------------------------------------------------------------
	bool stdfile_rstream_impl::pop_uint8_t(uint8_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		std::size_t								   sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = buffer[0];
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_uint16_t(uint16_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_read_uint16_t(buffer.data());
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_uint32_t(uint32_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_read_uint32_t(buffer.data());
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_uint64_t(uint64_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_read_uint64_t(buffer.data());
			return true;
		}
		return false;
	}

	bool stdfile_rstream_impl::pop_int8_t(int8_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		std::size_t								   sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_uts8(buffer[0]);
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_int16_t(int16_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_uts16(_serializer_utils::_read_uint16_t(buffer.data()));
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_int32_t(int32_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_uts32(_serializer_utils::_read_uint32_t(buffer.data()));
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_int64_t(int64_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_uts64(_serializer_utils::_read_uint64_t(buffer.data()));
			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------------------------------
	bool stdfile_rstream_impl::pop_float(float& out)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_utf(_serializer_utils::_read_uint32_t(buffer.data()));
			return true;
		}
		return false;
	}
	bool stdfile_rstream_impl::pop_double(double& out)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		if (sz == buffer.size())
		{
			out = _serializer_utils::_utd(_serializer_utils::_read_uint64_t(buffer.data()));
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	uint8_t stdfile_rstream_impl::pop_uint8_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		std::size_t								   sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return buffer[0];
	}
	uint16_t stdfile_rstream_impl::pop_uint16_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_read_uint16_t(buffer.data());
	}
	uint32_t stdfile_rstream_impl::pop_uint32_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_read_uint32_t(buffer.data());
	}
	uint64_t stdfile_rstream_impl::pop_uint64_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_read_uint64_t(buffer.data());
	}
	//--------------------------------------------------------------------------------------------------------------------
	int8_t stdfile_rstream_impl::pop_int8_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint8_t)> buffer;
		std::size_t								   sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_uts8(buffer[0]);
	}
	int16_t stdfile_rstream_impl::pop_int16_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint16_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_uts16(_serializer_utils::_read_uint16_t(buffer.data()));
	}
	int32_t stdfile_rstream_impl::pop_int32_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_uts32(_serializer_utils::_read_uint32_t(buffer.data()));
	}
	int64_t stdfile_rstream_impl::pop_int64_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_uts64(_serializer_utils::_read_uint64_t(buffer.data()));
	}
	//--------------------------------------------------------------------------------------------------------------------
	float stdfile_rstream_impl::pop_float()
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint32_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_utf(_serializer_utils::_read_uint32_t(buffer.data()));
	}
	double stdfile_rstream_impl::pop_double()
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::array<cllio::byte_t, sizeof(uint64_t)> buffer;
		std::size_t									sz = std::fread(buffer.data(), 1, buffer.size(), m_file_ptr);
		CLLIO_ASSERT(sz == buffer.size());
		return _serializer_utils::_utd(_serializer_utils::_read_uint64_t(buffer.data()));
	}
	//--------------------------------------------------------------------------------------------------------------------
}
