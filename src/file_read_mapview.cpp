
#include <file_read_mapview.h>

#ifdef PRJ_PLATFORM_IS_WIN32
#	include <memory>
#	include <io.h>
#	include <windows.h>
#	include <sys/types.h>
#	ifdef max
#		undef max
#	endif
#	ifdef min
#		undef min
#	endif
#endif

namespace cllio
{
	file_read_mapview::file_read_mapview(const std::filesystem::path& p)
		: file_read_mapview(p.string().c_str())
	{
	}

#ifdef PRJ_PLATFORM_IS_WIN32
	struct file_read_mapview_handle_impl
	{
		HANDLE file_handle;
		HANDLE file_mapping;

		static inline file_read_mapview_handle_impl& get_handle(file_read_mapview& h)
		{
			static_assert(sizeof(file_read_mapview_handle_impl) <= sizeof(h.m_handles), "Invalid handle");
			return *static_cast<file_read_mapview_handle_impl*>(static_cast<void*>(&h.m_handles[0]));
		}
		static inline const file_read_mapview_handle_impl& get_handle(const file_read_mapview& h)
		{
			static_assert(sizeof(file_read_mapview_handle_impl) <= sizeof(h.m_handles), "Invalid handle");
			return *static_cast<const file_read_mapview_handle_impl*>(static_cast<const void*>(&h.m_handles[0]));
		}
	};

	file_read_mapview::file_read_mapview(const char* abs_file_path)
	{
		this->m_data = nullptr;
		this->m_size = 0;
		auto& h = file_read_mapview_handle_impl::get_handle(*this);
		h.file_handle = NULL;
		h.file_mapping = NULL;

		h.file_handle = CreateFile(abs_file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
		if (h.file_handle == NULL)
			return;

		this->m_size = (std::size_t)GetFileSize(h.file_handle, NULL);
		DWORD flProtect = PAGE_READONLY;

		h.file_mapping = CreateFileMapping(h.file_handle, NULL, flProtect, 0, 0, NULL);
		if (h.file_mapping == NULL)
			return;

		// dwDesiredAccess = FILE_MAP_WRITE;
		DWORD dwDesiredAccess = FILE_MAP_READ;

		// dwDesiredAccess |= FILE_MAP_EXECUTE;
		this->m_data = (const void*)MapViewOfFile(h.file_mapping, dwDesiredAccess, 0, 0, 0);
	}
	file_read_mapview::~file_read_mapview()
	{
		if (this->m_data != nullptr)
			UnmapViewOfFile(this->m_data);

		auto& h = file_read_mapview_handle_impl::get_handle(*this);

		if (h.file_mapping != NULL)
			CloseHandle(h.file_mapping);

		if (h.file_handle != NULL)
			CloseHandle(h.file_handle);
	}
#endif

}