
#include <file_read_mapview.h>

#ifdef CLLIO_FILE_READ_MAPVIEW_WIN32
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

#ifdef CLLIO_FILE_READ_MAPVIEW_MMAP
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

	void file_read_mapview::swap(file_read_mapview& other)
	{
#ifdef CLLIO_FILE_READ_MAPVIEW_WIN32
		byte_t tmp[sizeof(void*) * 2];
		std::memcpy(tmp, this->m_handles, sizeof(tmp));
		std::memcpy(this->m_handles, other.m_handles, sizeof(tmp));
		std::memcpy(other.m_handles, tmp, sizeof(tmp));
#endif
#ifdef CLLIO_FILE_READ_MAPVIEW_MMAP
		std::swap(m_file_handle, other.m_file_handle);
#endif
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
	}

	file_read_mapview::file_read_mapview(file_read_mapview&& other) noexcept
	{
		other.swap(*this);
	}

	file_read_mapview& file_read_mapview::operator=(file_read_mapview&& other) noexcept
	{
		file_read_mapview tmp;
		tmp.swap(*this);
		other.swap(*this);
		return (*this);
	}

#ifdef CLLIO_FILE_READ_MAPVIEW_WIN32
	file_read_mapview_platform_impl::file_read_mapview_platform_impl()
	{
		std::memset(m_handles, 0, sizeof(m_handles));
	}
#	define HAS_IMPLEMENTATION
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
		auto& h = file_read_mapview_handle_impl::get_handle(*this);
		h.file_handle = NULL;
		h.file_mapping = NULL;

		HANDLE fh = CreateFile(abs_file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
		if (fh == NULL)
			return;

		std::size_t sz = (std::size_t)GetFileSize(fh, NULL);
		if (sz == 0)
		{
			CloseHandle(fh);
			return;
		}

		HANDLE mh = CreateFileMapping(fh, NULL, PAGE_READONLY, 0, 0, NULL);
		if (mh == NULL)
		{
			CloseHandle(fh);
			return;
		}

		h.file_handle = fh;
		h.file_mapping = mh;

		this->m_data = (const void*)MapViewOfFile(mh, FILE_MAP_READ, 0, 0, 0); // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-mapviewoffile
		this->m_size = sz;

		CLLIO_ASSERT(this->m_data != nullptr);
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

#ifdef CLLIO_FILE_READ_MAPVIEW_MMAP
#	define HAS_IMPLEMENTATION
	file_read_mapview::file_read_mapview(const char* abs_file_path)
	{
		m_file_handle = -1;

		int fh = open(abs_file_path, O_RDONLY);
		if (fh < 0)
			return;

		struct stat statbuf;
		int			err = fstat(fh, &statbuf);
		if (err < 0 || statbuf.st_size <= 0)
		{
			// stat failed close file also
			close(fh);
			return;
		}

		void* ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fh, 0);
		if (ptr == MAP_FAILED)
		{
			close(fh);
			return;
		}

		m_file_handle = fh;

		m_data = ptr;

		m_size = std::size_t(statbuf.st_size);
	}
	file_read_mapview::~file_read_mapview()
	{
		if (m_data != nullptr)
		{
			/* err = */ munmap(m_data, m_size);
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
#	undef HAS_IMPLEMENTATION
#else
#	error "No implementation for file_read_mapview, check platform"
#endif
