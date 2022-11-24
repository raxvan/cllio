#pragma once

#include "file_read_mapview.h"
#include "../stdfile_stream_io.h"
#include "../memory_stream_io.h"

namespace cllio
{

	struct stream_data_view
	{
		const cllio::byte_t* data = nullptr;
		const std::size_t	 size = 0;
	};

	//--------------------------------------------------------------------------------------------------------------------------------

	struct stream_reader
	{
	public:
		stream_reader() = default;
		stream_reader(const stream_reader&) = default;
		stream_reader& operator=(const stream_reader&) = default;

	public:
		stream_reader(const cllio::file_read_mapview&);
		stream_reader(const cllio::memory_rstream&);
		stream_reader(cllio::stdfile_rstream_impl&);

		stream_reader(std::FILE*);
		stream_reader(std::istream&);

		// TODO: stream_reader(cllio::memory_rstream&);
	public:
		stream_data_view read(std::vector<cllio::byte_t>& buffer); // append to buffer
		stream_data_view read();								   // can fail if used reader can't get direct memory

	protected:
		const cllio::file_read_mapview* m_map_file_handle = nullptr;
		const cllio::memory_rstream*	m_memory_handle = nullptr;
		cllio::stdfile_rstream_impl*	m_std_file_handle = nullptr;
		std::FILE*						m_raw_file_handle = nullptr;
		std::istream*					m_istream_handle = nullptr;
	};

	//--------------------------------------------------------------------------------------------------------------------------------

	struct stream_writer
	{
	public:
		stream_writer() = default;
		stream_writer(const stream_writer&) = default;
		stream_writer& operator=(const stream_writer&) = default;

	public:
		stream_writer(std::FILE*);
		stream_writer(std::ostream&);
		stream_writer(cllio::stdfile_wstream_impl&);

		// TODO: stream_writer(cllio::memory_wstream&);

	public:
		void write(const void* data, const std::size_t sz);

	protected:
		cllio::stdfile_wstream_impl* m_std_file_handle = nullptr;
		std::FILE*					 m_raw_file_handle = nullptr;
		std::ostream*				 m_istream_handle = nullptr;
	};

	//--------------------------------------------------------------------------------------------------------------------------------
}
