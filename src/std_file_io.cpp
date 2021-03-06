

#include "std_file_io.h"
#include "cllio_internal.h"

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
	std_file_writer_impl::std_file_writer_impl(std::FILE* handle)
		: std_file_handle(handle)
	{
	}
	std_file_reader_impl::std_file_reader_impl(std::FILE* handle)
		: std_file_handle(handle)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool std_file_writer_impl::create(const char* abs_path, const bool binary, const bool append)
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

	void std_file_writer_impl::flush()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::fflush(m_file_ptr);
	}

	//--------------------------------------------------------------------------------------------------------------------
	void std_file_writer_impl::push_int8_t(const int8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_int16_t(const int16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_int32_t(const int32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_int64_t(const int64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}

	void std_file_writer_impl::push_uint8_t(const uint8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_uint16_t(const uint16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_uint32_t(const uint32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_uint64_t(const uint64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(value));
		consider_it_used(sz);
	}
	void std_file_writer_impl::push_float(const float value)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		UnionCast<float, uint32_t> data;
		data.first = value;
		push_uint32_t(data.second);
	}
	void std_file_writer_impl::push_double(const double value)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		UnionCast<double, uint64_t> data;
		data.first = value;
		push_uint64_t(data.second);
	}
	void std_file_writer_impl::push_ptr(const void* px)
	{
		UnionCast<uint64_t, const void*> data;
		data.first = 0;
		data.second = px;
		push_uint64_t(data.first);
	}
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	bool std_file_writer_impl::trypush_int8_t(const int8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_int16_t(const int16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_int32_t(const int32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_int64_t(const int64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}

	bool std_file_writer_impl::trypush_uint8_t(const uint8_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_uint16_t(const uint16_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_uint32_t(const uint32_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_uint64_t(const uint64_t value)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(&value, 1, sizeof(value), m_file_ptr);
		return (sz == sizeof(value));
	}
	bool std_file_writer_impl::trypush_float(const float value)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		UnionCast<float, uint32_t> data;
		data.first = value;
		return trypush_uint32_t(data.second);
	}
	bool std_file_writer_impl::trypush_double(const double value)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		UnionCast<double, uint64_t> data;
		data.first = value;
		return trypush_uint64_t(data.second);
	}
	bool std_file_writer_impl::trypush_ptr(const void* px)
	{
		UnionCast<uint64_t, const void*> data;
		data.first = 0;
		data.second = px;
		return trypush_uint64_t(data.first);
	}

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------

	void std_file_writer_impl::write_raw_buffer(const void* data, const std::size_t byte_count)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(data, 1, byte_count, m_file_ptr);
		CLLIO_ASSERT(sz == byte_count);
		consider_it_used(sz);
	}
	bool std_file_writer_impl::trywrite_raw_buffer(const void* data, const std::size_t byte_count)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fwrite(data, 1, byte_count, m_file_ptr);
		return (sz == byte_count);
	}

	//--------------------------------------------------------------------------------------------------------------------

	std::size_t std_file_writer_impl::str_printf(const char* format, ...)
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

	std::size_t std_file_writer_impl::str(const char* format)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		auto r = std::fprintf(m_file_ptr, "%s", format);
		if (r < 0)
			r = 0;
		return static_cast<std::size_t>(r);
	}
	std::size_t std_file_writer_impl::str(const int32_t v)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		auto r = std::fprintf(m_file_ptr, "%i", v);
		if (r < 0)
			r = 0;
		return static_cast<std::size_t>(r);
	}
	std::size_t std_file_writer_impl::str(const uint32_t v)
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
	bool std_file_reader_impl::open(const char* abs_path, const bool is_binary)
	{
		if (is_binary)
			return open_binary(abs_path);
		return open(abs_path);
	}
	bool std_file_reader_impl::open(const char* abs_path)
	{
		if (m_file_ptr != nullptr)
			std::fclose(m_file_ptr);
#ifdef _MSC_VER
		errno_t err = fopen_s(&m_file_ptr, abs_path, "r");
		if (err == 0)
		{
			return true; // ok
		}
		m_file_ptr = nullptr;
		return false;
#else
		m_file_ptr = std::fopen(abs_path, "r");
		return m_file_ptr != nullptr;
#endif
	}
	bool std_file_reader_impl::open_binary(const char* abs_path)
	{
		if (m_file_ptr != nullptr)
			std::fclose(m_file_ptr);
#ifdef _MSC_VER
		errno_t err = fopen_s(&m_file_ptr, abs_path, "rb");
		if (err == 0)
		{
			return true; // ok
		}
		m_file_ptr = nullptr;
		return false;
#else
		m_file_ptr = std::fopen(abs_path, "rb");
		return m_file_ptr != nullptr;
#endif
	}

	//--------------------------------------------------------------------------------------------------------------------

	void std_file_reader_impl::read_raw_buffer(void* dest, const std::size_t ammount)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fread(dest, 1, ammount, m_file_ptr);
		CLLIO_ASSERT(sz == ammount);
		consider_it_used(sz);
	}
	bool std_file_reader_impl::tryread_raw_buffer(void* dest, const std::size_t ammount)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		std::size_t sz = std::fread(dest, 1, ammount, m_file_ptr);
		return (sz == ammount);
	}
	

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	template <class T>
	inline bool _std_file_read_v0(std::FILE* f, T& out)
	{
		T tmpbuf;
		if (sizeof(T) != std::fread(&tmpbuf, 1, sizeof(T), f))
			return false;
		out = tmpbuf;
		return true;
	}
	template <class T>
	inline T _std_file_read_v1(std::FILE* f)
	{
		T			tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(T), f);
		CLLIO_ASSERT(sz == sizeof(T));
		consider_it_used(sz);
		return tmpbuf;
	}
	template <class T>
	inline T _std_file_read_v2(std::FILE* f, bool& error)
	{
		T tmpbuf;
		if (error)
			return T(0);
		if (sizeof(T) != std::fread(&tmpbuf, 1, sizeof(T), f))
		{
			error = true;
			return T(0);
		}
		return tmpbuf;
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool std_file_reader_impl::pop_uint8_t(uint8_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<uint8_t>(m_file_ptr, out);
	}
	bool std_file_reader_impl::pop_uint16_t(uint16_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<uint16_t>(m_file_ptr, out);
	}
	bool std_file_reader_impl::pop_uint32_t(uint32_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<uint32_t>(m_file_ptr, out);
	}
	bool std_file_reader_impl::pop_uint64_t(uint64_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<uint64_t>(m_file_ptr, out);
	}

	bool std_file_reader_impl::pop_int8_t(int8_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<int8_t>(m_file_ptr, out);
	}
	bool std_file_reader_impl::pop_int16_t(int16_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<int16_t>(m_file_ptr, out);
	}
	bool std_file_reader_impl::pop_int32_t(int32_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<int32_t>(m_file_ptr, out);
	}
	bool std_file_reader_impl::pop_int64_t(int64_t& out)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v0<int64_t>(m_file_ptr, out);
	}
	//--------------------------------------------------------------------------------------------------------------------
	uint8_t std_file_reader_impl::pop_uint8_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint8_t>(m_file_ptr, error);
	}
	uint16_t std_file_reader_impl::pop_uint16_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint16_t>(m_file_ptr, error);
	}
	uint32_t std_file_reader_impl::pop_uint32_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint32_t>(m_file_ptr, error);
	}
	uint64_t std_file_reader_impl::pop_uint64_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint64_t>(m_file_ptr, error);
	}

	int8_t std_file_reader_impl::pop_int8_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int8_t>(m_file_ptr, error);
	}
	int16_t std_file_reader_impl::pop_int16_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int16_t>(m_file_ptr, error);
	}
	int32_t std_file_reader_impl::pop_int32_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int32_t>(m_file_ptr, error);
	}
	int64_t std_file_reader_impl::pop_int64_t(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int64_t>(m_file_ptr, error);
	}
	float std_file_reader_impl::pop_float(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<float>(m_file_ptr, error);
	}
	double std_file_reader_impl::pop_double(bool& error)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<double>(m_file_ptr, error);
	}

	//--------------------------------------------------------------------------------------------------------------------
	bool std_file_reader_impl::pop_float(float& out)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint32_t tmpbuf;
		if (sizeof(tmpbuf) != std::fread(&tmpbuf, 1, sizeof(uint32_t), m_file_ptr))
			return false;

		UnionCast<uint32_t, float> data;
		data.first = tmpbuf;
		out = data.second;
		return true;
	}
	bool std_file_reader_impl::pop_double(double& out)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint64_t tmpbuf;
		if (sizeof(tmpbuf) != std::fread(&tmpbuf, 1, sizeof(uint64_t), m_file_ptr))
			return false;

		UnionCast<uint64_t, double> data;
		data.first = tmpbuf;
		out = data.second;
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	uint8_t std_file_reader_impl::pop_uint8_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<uint8_t>(m_file_ptr);
	}
	uint16_t std_file_reader_impl::pop_uint16_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<uint16_t>(m_file_ptr);
	}
	uint32_t std_file_reader_impl::pop_uint32_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<uint32_t>(m_file_ptr);
	}
	uint64_t std_file_reader_impl::pop_uint64_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<uint64_t>(m_file_ptr);
	}
	//--------------------------------------------------------------------------------------------------------------------
	int8_t std_file_reader_impl::pop_int8_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<int8_t>(m_file_ptr);
	}
	int16_t std_file_reader_impl::pop_int16_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<int16_t>(m_file_ptr);
	}
	int32_t std_file_reader_impl::pop_int32_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<int32_t>(m_file_ptr);
	}
	int64_t std_file_reader_impl::pop_int64_t()
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v1<int64_t>(m_file_ptr);
	}
	//--------------------------------------------------------------------------------------------------------------------
	float std_file_reader_impl::pop_float()
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint32_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(tmpbuf));
		consider_it_used(sz);
		UnionCast<uint32_t, float> data;
		data.first = tmpbuf;
		return data.second;
	}
	double std_file_reader_impl::pop_double()
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint64_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		CLLIO_ASSERT(sz == sizeof(tmpbuf));
		consider_it_used(sz);
		UnionCast<uint64_t, double> data;
		data.first = tmpbuf;
		return data.second;
	}
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------

	uint8_t std_file_reader_impl::popdefault_uint8_t(const uint8_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint8_t		tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	uint16_t std_file_reader_impl::popdefault_uint16_t(const uint16_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint16_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	uint32_t std_file_reader_impl::popdefault_uint32_t(const uint32_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint32_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	uint64_t std_file_reader_impl::popdefault_uint64_t(const uint64_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint64_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	//--------------------------------------------------------------------------------------------------------------------
	int8_t std_file_reader_impl::popdefault_int8_t(const int8_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		int8_t		tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	int16_t std_file_reader_impl::popdefault_int16_t(const int16_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		int16_t		tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	int32_t std_file_reader_impl::popdefault_int32_t(const int32_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		int32_t		tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	int64_t std_file_reader_impl::popdefault_int64_t(const int64_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		int64_t		tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		return tmpbuf;
	}
	//--------------------------------------------------------------------------------------------------------------------
	float std_file_reader_impl::popdefault_float(const float _default)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint32_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		UnionCast<uint32_t, float> data;
		data.first = tmpbuf;
		return data.second;
	}
	double std_file_reader_impl::popdefault_double(const double _default)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint64_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
			return _default;
		UnionCast<uint64_t, double> data;
		data.first = tmpbuf;
		return data.second;
	}

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	template <class T>
	inline bool _std_file_read_v2(std::FILE* f, T& out, const T _default)
	{
		T			tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), f);
		if (sz != sizeof(tmpbuf))
		{
			out = _default;
			return false;
		}
		out = tmpbuf;
		return true;
	}

	bool std_file_reader_impl::popdefault_uint8_t(uint8_t& out, const uint8_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint8_t>(m_file_ptr, out, _default);
	}
	bool std_file_reader_impl::popdefault_uint16_t(uint16_t& out, const uint16_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint16_t>(m_file_ptr, out, _default);
	}
	bool std_file_reader_impl::popdefault_uint32_t(uint32_t& out, const uint32_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint32_t>(m_file_ptr, out, _default);
	}
	bool std_file_reader_impl::popdefault_uint64_t(uint64_t& out, const uint64_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<uint64_t>(m_file_ptr, out, _default);
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool std_file_reader_impl::popdefault_int8_t(int8_t& out, const int8_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int8_t>(m_file_ptr, out, _default);
	}
	bool std_file_reader_impl::popdefault_int16_t(int16_t& out, const int16_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int16_t>(m_file_ptr, out, _default);
	}
	bool std_file_reader_impl::popdefault_int32_t(int32_t& out, const int32_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int32_t>(m_file_ptr, out, _default);
	}
	bool std_file_reader_impl::popdefault_int64_t(int64_t& out, const int64_t _default)
	{
		CLLIO_ASSERT(m_file_ptr != nullptr);
		return _std_file_read_v2<int64_t>(m_file_ptr, out, _default);
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool std_file_reader_impl::popdefault_float(float& out, const float _default)
	{
		static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint32_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
		{
			out = _default;
			return false;
		}
		UnionCast<uint32_t, float> data;
		data.first = tmpbuf;
		out = data.second;
		return true;
	}
	bool std_file_reader_impl::popdefault_double(double& out, const double _default)
	{
		static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
		CLLIO_ASSERT(m_file_ptr != nullptr);
		uint64_t	tmpbuf;
		std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
		if (sz != sizeof(tmpbuf))
		{
			out = _default;
			return false;
		}
		UnionCast<uint64_t, double> data;
		data.first = tmpbuf;
		out = data.second;
		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------
}
