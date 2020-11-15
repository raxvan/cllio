
#pragma once

#include "cllio_utils.h"
#include <filesystem>

namespace cllio
{
	struct file_read_mapview
	{
	public:
		file_read_mapview() = default;
		file_read_mapview(const file_read_mapview&) = delete;
		file_read_mapview& operator=(const file_read_mapview&) = delete;

	public:
		file_read_mapview(const std::filesystem::path& p);
		file_read_mapview(const char* abs_file_path);
		~file_read_mapview();

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
		const void* m_data = nullptr;
		std::size_t m_size = 0;

	protected:
#ifdef PRJ_PLATFORM_IS_WIN32
		byte_t m_handles[sizeof(void*) * 2];
		friend struct file_read_mapview_handle_impl;
#endif
	};

}