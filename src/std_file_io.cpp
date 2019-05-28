

#include "std_file_io.h"
#include "cllio_utils.h"

namespace cllio
{

std_file_handle::~std_file_handle()
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);
}
void std_file_handle::close()
{
	if (m_file_ptr != nullptr)
	{
		std::fclose(m_file_ptr);
		m_file_ptr = nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------
std_file_writer::std_file_writer(std_file_writer&& other)
{
	m_file_ptr = other.m_file_ptr;
	other.m_file_ptr = nullptr;
}
std_file_writer& std_file_writer::operator = (std_file_writer && other)
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);
	m_file_ptr = other.m_file_ptr;
	other.m_file_ptr = nullptr;
	return (*this);
}

bool		std_file_writer::open(const char * abs_path, const bool binary, const bool append)
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);
	errno_t err;

	if (append)
	{
		if(binary)
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
		return true;//ok
	}
	m_file_ptr = nullptr;
	return false;
}


void std_file_writer::flush()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	std::fflush(m_file_ptr);
}

//--------------------------------------------------------------------------------------------------------------------

void		std_file_writer::push_int8_t(const int8_t value)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	std::fwrite(&value, 1, sizeof(int8_t), m_file_ptr);
}
void		std_file_writer::push_int16_t(const int16_t value)
{
	UnionCast<int16_t,uint16_t> data;
	data.first = value;
	push_uint16_t(data.second);
}
void		std_file_writer::push_int32_t(const int32_t value)
{
	UnionCast<int32_t,uint32_t> data;
	data.first = value;
	push_uint32_t(data.second);
}
void		std_file_writer::push_int64_t(const int64_t value)
{
	UnionCast<int64_t,uint64_t> data;
	data.first = value;
	push_uint64_t(data.second);
}

void		std_file_writer::push_uint8_t(const uint8_t value)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	std::fwrite(&value, 1, sizeof(uint8_t), m_file_ptr);
}
void		std_file_writer::push_uint16_t(const uint16_t value)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	detail::uint16_to_binary(tmpbuf, value);
	std::fwrite(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
}
void		std_file_writer::push_uint32_t(const uint32_t value)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	detail::uint32_to_binary(tmpbuf, value);
	std::fwrite(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
}
void		std_file_writer::push_uint64_t(const uint64_t value)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	detail::uint64_to_binary(tmpbuf, value);
	std::fwrite(&tmpbuf.data[0], tmpbuf.size(), 1, m_file_ptr);
}
void		std_file_writer::push_float(const float value)
{
	static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
	UnionCast<float, uint32_t> data;
	data.first = value;
	push_uint32_t(data.second);
}
void		std_file_writer::push_double(const double value)
{
	static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
	UnionCast<double, uint64_t> data;
	data.first = value;
	push_uint64_t(data.second);
}

//--------------------------------------------------------------------------------------------------------------------

void		std_file_writer::buffer_write(const void * data, const std::size_t byte_count)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	std::fwrite(data, 1, byte_count, m_file_ptr);
}

//--------------------------------------------------------------------------------------------------------------------

std::size_t std_file_writer::str_printf(const char * format, ...)
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

std::size_t		std_file_writer::str(const char * format)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	auto r = std::fprintf(m_file_ptr, "%s", format);
	if (r < 0)
		r = 0;
	return static_cast<std::size_t>(r);
}
std::size_t		std_file_writer::str(const int32_t v)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	auto r = std::fprintf(m_file_ptr, "%i", v);
	if (r < 0)
		r = 0;
	return static_cast<std::size_t>(r);
}
std::size_t		std_file_writer::str(const uint32_t v)
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
std_file_reader::std_file_reader(std_file_reader&& other)
{
	m_file_ptr = other.m_file_ptr;
	other.m_file_ptr = nullptr;
}
std_file_reader& std_file_reader::operator = (std_file_reader && other)
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);
	m_file_ptr = other.m_file_ptr;
	other.m_file_ptr = nullptr;
	return (*this);
}
bool std_file_reader::open(const char * abs_path, const bool is_binary)
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);

	errno_t err;

	if (is_binary)
		err = fopen_s(&m_file_ptr, abs_path, "rb");
	else
		err = fopen_s(&m_file_ptr, abs_path, "r");

	if (err == 0)
	{
		return true;//ok
	}
	m_file_ptr = nullptr;
	return false;
}
bool std_file_reader::open(const char * abs_path)
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);
	errno_t err = fopen_s(&m_file_ptr, abs_path, "r");
	if (err == 0)
	{
		return true;//ok
	}
	m_file_ptr = nullptr;
	return false;
}
bool std_file_reader::open_binary(const char * abs_path)
{
	if (m_file_ptr != nullptr)
		std::fclose(m_file_ptr);
	errno_t err = fopen_s(&m_file_ptr, abs_path, "rb");
	if (err == 0)
	{
		return true;//ok
	}
	m_file_ptr = nullptr;
	return false;
}
std::size_t std_file_reader::get_file_size()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	std::size_t cursor = std::ftell(m_file_ptr);
	std::fseek(m_file_ptr, 0, SEEK_END);
	std::size_t filesize = std::ftell(m_file_ptr);

	std::fseek(m_file_ptr, cursor, SEEK_SET);
	return filesize;
}

//--------------------------------------------------------------------------------------------------------------------

bool std_file_reader::buffer_read(void * dest, const std::size_t ammount)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	std::size_t sz = std::fread(dest, 1, ammount, m_file_ptr);
	return sz == ammount;
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
bool std_file_reader::pop_uint8_t(uint8_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	uint8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf), m_file_ptr);
	if (sz != sizeof(tmpbuf))
		return false;
	out = tmpbuf;
	return true;
}
bool std_file_reader::pop_uint16_t(uint16_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	out = detail::uint16_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::pop_uint32_t(uint32_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	out = detail::uint32_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::pop_uint64_t(uint64_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	out = detail::uint64_from_binary(tmpbuf);
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
bool std_file_reader::pop_int8_t(int8_t& out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	int8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(int8_t), m_file_ptr);
	if (sz != sizeof(int8_t))
		return false;
	out = tmpbuf;
	return true;
}
bool std_file_reader::pop_int16_t(int16_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	out = detail::int16_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::pop_int32_t(int32_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	out = detail::int32_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::pop_int64_t(int64_t & out)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	out = detail::int64_from_binary(tmpbuf);
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
bool std_file_reader::pop_float(float & out)
{
	static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	UnionCast<uint32_t,float> data;
	data.first = detail::uint32_from_binary(tmpbuf);
	out = data.second;
	return true;
}
bool std_file_reader::pop_double(double & out)
{
	static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
		return false;
	UnionCast<uint64_t,double> data;
	data.first = detail::uint64_from_binary(tmpbuf);
	out = data.second;
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
uint8_t std_file_reader::pop_uint8_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	uint8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf) , m_file_ptr);
	CLLIO_ASSERT (sz == sizeof(tmpbuf));
	CLLIO_UNUSED(sz);
	return tmpbuf;
}
uint16_t std_file_reader::pop_uint16_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	return detail::uint16_from_binary(tmpbuf);
}
uint32_t std_file_reader::pop_uint32_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	return detail::uint32_from_binary(tmpbuf);
}
uint64_t std_file_reader::pop_uint64_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	return detail::uint64_from_binary(tmpbuf);
}
//--------------------------------------------------------------------------------------------------------------------
int8_t std_file_reader::pop_int8_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	int8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(int8_t), m_file_ptr);
	CLLIO_ASSERT (sz == sizeof(int8_t));
	CLLIO_UNUSED(sz);
	return tmpbuf;
}
int16_t std_file_reader::pop_int16_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	return detail::int16_from_binary(tmpbuf);
}
int32_t std_file_reader::pop_int32_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	return detail::int32_from_binary(tmpbuf);
}
int64_t std_file_reader::pop_int64_t()
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	return detail::int64_from_binary(tmpbuf);
}
//--------------------------------------------------------------------------------------------------------------------
float std_file_reader::pop_float()
{
	static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	UnionCast<uint32_t,float> data;
	data.first = detail::uint32_from_binary(tmpbuf);
	return data.second;
}
double std_file_reader::pop_double()
{
	static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	CLLIO_ASSERT (sz == tmpbuf.size());
	CLLIO_UNUSED(sz);
	UnionCast<uint64_t,double> data;
	data.first = detail::uint64_from_binary(tmpbuf);
	return data.second;
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

uint8_t std_file_reader::popdefault_uint8_t(const uint8_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	uint8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(uint8_t) , m_file_ptr);
	if (sz != sizeof(uint8_t))
		return _default;
	return tmpbuf;
}
uint16_t std_file_reader::popdefault_uint16_t(const uint16_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	return detail::uint16_from_binary(tmpbuf);
}
uint32_t std_file_reader::popdefault_uint32_t(const uint32_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	return detail::uint32_from_binary(tmpbuf);
}
uint64_t std_file_reader::popdefault_uint64_t(const uint64_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	return detail::uint64_from_binary(tmpbuf);
}
//--------------------------------------------------------------------------------------------------------------------
int8_t std_file_reader::popdefault_int8_t(const int8_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	int8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(int8_t), m_file_ptr);
	if (sz != sizeof(int8_t))
		return _default;
	return tmpbuf;
}
int16_t std_file_reader::popdefault_int16_t(const int16_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	return detail::int16_from_binary(tmpbuf);
}
int32_t std_file_reader::popdefault_int32_t(const int32_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	return detail::int32_from_binary(tmpbuf);
}
int64_t std_file_reader::popdefault_int64_t(const int64_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	return detail::int64_from_binary(tmpbuf);
}
//--------------------------------------------------------------------------------------------------------------------
float std_file_reader::popdefault_float(const float _default)
{
	static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	UnionCast<uint32_t,float> data;
	data.first = detail::uint32_from_binary(tmpbuf);
	return data.second;
}
double std_file_reader::popdefault_double(const double _default)
{
	static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
		return _default;
	UnionCast<uint64_t,double> data;
	data.first = detail::uint64_from_binary(tmpbuf);
	return data.second;
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

bool std_file_reader::popdefault_uint8_t(uint8_t & out, const uint8_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	uint8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(tmpbuf) , m_file_ptr);
	if (sz != sizeof(tmpbuf))
	{
		out = _default;
		return false;
	}
	out = tmpbuf;
	return true;
}
bool std_file_reader::popdefault_uint16_t(uint16_t & out, const uint16_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2, uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	out = detail::uint16_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::popdefault_uint32_t(uint32_t & out, const uint32_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	out = detail::uint32_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::popdefault_uint64_t(uint64_t & out, const uint64_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	out = detail::uint64_from_binary(tmpbuf);
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
bool std_file_reader::popdefault_int8_t(int8_t & out, const int8_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	int8_t tmpbuf;
	std::size_t sz = std::fread(&tmpbuf, 1, sizeof(int8_t), m_file_ptr);
	if (sz != sizeof(int8_t))
	{
		out = _default;
		return false;
	}
	out = tmpbuf;
	return true;
}
bool std_file_reader::popdefault_int16_t(int16_t & out, const int16_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<2,uint16_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	out = detail::int16_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::popdefault_int32_t(int32_t & out, const int32_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	out = detail::int32_from_binary(tmpbuf);
	return true;
}
bool std_file_reader::popdefault_int64_t(int64_t & out, const int64_t _default)
{
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	out = detail::int64_from_binary(tmpbuf);
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
bool std_file_reader::popdefault_float(float & out, const float _default)
{
	static_assert(sizeof(float) <= sizeof(uint32_t), "sizeof(float) > 4 bytes?");
	detail::buffer<4,uint32_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size() , m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	UnionCast<uint32_t,float> data;
	data.first = detail::uint32_from_binary(tmpbuf);
	out = data.second;
	return true;
}
bool std_file_reader::popdefault_double(double & out, const double _default)
{
	static_assert(sizeof(double) <= sizeof(uint64_t), "sizeof(double) > 8 bytes?");
	CLLIO_ASSERT(m_file_ptr != nullptr);
	detail::buffer<8,uint64_t> tmpbuf;
	std::size_t sz = std::fread(&tmpbuf.data[0], 1, tmpbuf.size(), m_file_ptr);
	if (sz != tmpbuf.size())
	{
		out = _default;
		return false;
	}
	UnionCast<uint64_t,double> data;
	data.first = detail::uint64_from_binary(tmpbuf);
	out = data.second;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------

}