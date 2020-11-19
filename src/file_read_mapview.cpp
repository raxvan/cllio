
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

#ifdef PRJ_PLATFORM_IS_LINUX
#	include <stdio.h>
#	include <sys/mman.h>
#	include <stdlib.h>
#	include <sys/stat.h>
#	include <fcntl.h>
#	include <unistd.h>
#endif

namespace cllio
{
	file_read_mapview::file_read_mapview(const std::filesystem::path& p)
		: file_read_mapview(p.string().c_str())
	{
	}

#ifdef PRJ_PLATFORM_IS_WIN32
	#define HAS_IMPLEMENTATION
	struct file_read_mapview_handle_impl
	{
		HANDLE file_handle;
		HANDLE file_mapping;

		static inline file_read_mapview_handle_impl& get_handle(file_read_mapview_platform_impl& h)
		{
			static_assert(sizeof(file_read_mapview_handle_impl) <= sizeof(h.m_handles), "Invalid handle");
			return *static_cast<file_read_mapview_handle_impl*>(static_cast<void*>(&h.m_handles[0]));
		}
		static inline const file_read_mapview_handle_impl& get_handle(const file_read_mapview_platform_impl& h)
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

#ifdef PRJ_PLATFORM_IS_LINUX
	#define HAS_IMPLEMENTATION
	file_read_mapview::file_read_mapview(const char* abs_file_path)
	{
		this->m_data = nullptr;
		this->m_size = 0;

		m_file_handle = -1;
		m_mmap_handle = nullptr;

		m_file_handle = open(abs_file_path, O_RDONLY);
		if (m_file_handle < 0)
			return;

		struct stat statbuf;
		int			err = fstat(m_file_handle, &statbuf);
		if (err < 0)
			return;

		void* ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, m_file_handle, 0);
		if (ptr == MAP_FAILED)
		{
			return;
		}
		else
		{
			m_mmap_handle = ptr;
		}

		m_data = ptr;
		m_size = std::size_t(statbuf.st_size);
	}
	file_read_mapview::~file_read_mapview()
	{
		if (m_mmap_handle != nullptr)
		{
			/* err = */ munmap(m_mmap_handle, m_size);
			/*if (err != 0) {
				printf("UnMapping Failed\n");
			}
			*/
		}

		if (m_file_handle >= 0)
			close(m_file_handle);
	}
#endif

}

#ifdef HAS_IMPLEMENTATION
	#undef HAS_IMPLEMENTATION
#else
	#error "No implementation for file_read_mapview, check platform"
#endif