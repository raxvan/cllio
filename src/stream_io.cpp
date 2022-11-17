#include <cllio_tools/stream_io.h>
#include "cllio_internal.h"

namespace cllio
{

	stream_reader::stream_reader(const cllio::file_read_mapview& h)
		: m_map_file_handle(&h)
	{
	}
	stream_reader::stream_reader(const cllio::memory_rstream& h)
		: m_memory_handle(&h)
	{
	}
	stream_reader::stream_reader(cllio::stdfile_rstream_impl& h)
		: m_std_file_handle(&h)
	{
	}
	stream_reader::stream_reader(std::istream& h)
		: m_istream_handle(&h)
	{
	}
	stream_reader::stream_reader(std::FILE* h)
		: m_raw_file_handle(h)
	{
	}

	stream_data_view stream_reader::read(std::vector<cllio::byte_t>& buffer)
	{
		{
			auto r = read();
			if (r.size > 0)
				return r;
		}
		std::size_t start = buffer.size();
		if (m_std_file_handle != nullptr)
		{
			m_std_file_handle->read_into_container(buffer);
		}
		else if (m_raw_file_handle != nullptr)
		{
			auto pos = std::ftell(m_raw_file_handle);
			int icheck;
			std::size_t scheck;
			icheck = std::fseek(m_raw_file_handle, 0, SEEK_END);
			CLLIO_ASSERT(icheck == 0);
			std::size_t filesize = std::ftell(m_raw_file_handle);
			icheck = std::fseek(m_raw_file_handle, pos, SEEK_SET);
			CLLIO_ASSERT(icheck == 0);
			std::size_t sz = std::size_t(filesize) - std::size_t(pos);
			buffer.resize(start + sz);
			scheck = std::fread(buffer.data() + start, 1, sz, m_raw_file_handle);
			CLLIO_ASSERT(scheck == sz);

			consider_it_used(icheck);
			consider_it_used(scheck);
			
		}
		else if (m_istream_handle != nullptr)
		{
			// TODO: check performance on this!
			buffer.insert(buffer.end(), std::istreambuf_iterator<char>(*m_istream_handle), std::istreambuf_iterator<char>());
		}

		return stream_data_view(buffer.data() + start, buffer.size() - start);
	}
	stream_data_view stream_reader::read()
	{
		if (m_map_file_handle != nullptr)
		{
			return stream_data_view { static_cast<const cllio::byte_t*>(m_map_file_handle->data()), m_map_file_handle->size() };
		}
		if (m_memory_handle != nullptr)
		{
			return stream_data_view { m_memory_handle->data(), std::size_t(m_memory_handle->size()) };
		}
		return stream_data_view{};
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	stream_writer::stream_writer(cllio::stdfile_wstream_impl& h)
		: m_std_file_handle(&h)
	{
	}
	stream_writer::stream_writer(std::FILE* h)
		: m_raw_file_handle(h)
	{
	}
	stream_writer::stream_writer(std::ostream& h)
		: m_istream_handle(&h)
	{
	}

	void stream_writer::write(const void* data, const std::size_t sz)
	{
		if (m_std_file_handle != nullptr)
			m_std_file_handle->write_raw_buffer(data, sz);
		else if (m_raw_file_handle != nullptr)
			std::fwrite(data, 1, sz, m_raw_file_handle);
		else if (m_istream_handle != nullptr)
			m_istream_handle->write((const char*)data, sz);
	}

}
