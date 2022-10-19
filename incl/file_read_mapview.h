
#pragma once

#include "cllio_utils.h"
#include <filesystem>

#ifndef CLLIO_FILE_READ_MAPVIEW
#	ifdef _WIN32
#		define CLLIO_FILE_READ_MAPVIEW_WIN32
#	else
#		define CLLIO_FILE_READ_MAPVIEW_MMAP
#	endif
#endif

namespace cllio
{

	struct file_read_mapview_platform_impl
	{
#ifdef CLLIO_FILE_READ_MAPVIEW_WIN32
		byte_t m_handles[sizeof(void*) * 2];
		friend struct file_read_mapview_handle_impl;
		file_read_mapview_platform_impl();
#endif

#ifdef CLLIO_FILE_READ_MAPVIEW_MMAP
		int m_file_handle = 0;
#endif
	};

	struct file_read_mapview : protected file_read_mapview_platform_impl
	{
	public:
		file_read_mapview() = default;
		file_read_mapview(const file_read_mapview&) = delete;
		file_read_mapview& operator=(const file_read_mapview&) = delete;

		file_read_mapview(file_read_mapview&&) noexcept;
		file_read_mapview& operator=(file_read_mapview&&) noexcept;

	public:
		file_read_mapview(const std::filesystem::path& p);
		file_read_mapview(const char* abs_file_path);
		~file_read_mapview();

		void swap(file_read_mapview& other);

	public:
		inline const void* data() const
		{
			return m_data;
		}
		inline std::size_t size() const
		{
			return m_size;
		}

	protected:
#ifdef CLLIO_FILE_READ_MAPVIEW_WIN32
		const void* m_data = nullptr;
#endif

#ifdef CLLIO_FILE_READ_MAPVIEW_MMAP
		void* m_data = nullptr;
#endif

	protected:
		std::size_t m_size = 0;
	};

}
