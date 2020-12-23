
#pragma once

#include "cllio_utils.h"
#include <filesystem>

namespace cllio
{

	struct file_read_mapview_platform_impl
	{
#ifdef PRJ_PLATFORM_IS_WIN32
		byte_t m_handles[sizeof(void*) * 2];
		friend struct file_read_mapview_handle_impl;
		file_read_mapview_platform_impl();
#endif

#ifdef PRJ_PLATFORM_IS_LINUX
		int	  m_file_handle = 0;
#endif
	};

	struct file_read_mapview : public file_read_mapview_platform_impl
	{
	public:
		file_read_mapview() = default;
		file_read_mapview(const file_read_mapview&) = delete;
		file_read_mapview& operator=(const file_read_mapview&) = delete;

		file_read_mapview(file_read_mapview&&);
		file_read_mapview& operator=(file_read_mapview&&);

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

#ifdef PRJ_PLATFORM_IS_WIN32
		const void* m_data = nullptr;
#else
		void* m_data = nullptr;
#endif

		std::size_t m_size = 0;
	};

}