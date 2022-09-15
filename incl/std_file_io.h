#pragma once

#include "cllio_internal_utils.h"

namespace cllio
{

	// fwd declare
	struct std_file_reader_impl;
	struct std_file_writer_impl;

	// usings:
	// using std_file_read_view -> non owning
	// using std_file_write_view -> non owning;
	// using std_file_read -> owning;
	// using std_file_write -> owning;

	//-----------------------------------------------------------------------------------------------------------
	struct std_file_handle
	{
	public:
		std_file_handle() = default;
		~std_file_handle();

	public:
		std_file_handle(const std_file_handle&) = delete;
		std_file_handle& operator=(const std_file_handle&) = delete;

		bool isOpen() const;

		bool close();

		std::size_t get_file_size();
		std::size_t get_remaining_size();

		std::FILE* get_handle();

		void swap(std_file_handle& other);

	protected:
		std_file_handle(std::FILE* handle);

		std::FILE* m_file_ptr = nullptr;
	};
	//-----------------------------------------------------------------------------------------------------------
	struct std_file_writer_impl : public std_file_handle
	{
	public:
		std_file_writer_impl() = default;
		std_file_writer_impl(const std_file_writer_impl&) = delete;
		std_file_writer_impl& operator=(const std_file_writer_impl&) = delete;

	protected:
		std_file_writer_impl(std::FILE* handle);

	public:
		bool create(const char* abs_path, const bool binary = true, const bool append = false);
		void flush();

	public: // binary writers, no error check
		void write_raw_buffer(const void* data, const std::size_t byte_count);
		bool trywrite_raw_buffer(const void* data, const std::size_t byte_count);
		
	public:
		void push_int8_t(const int8_t value);
		void push_int16_t(const int16_t value);
		void push_int32_t(const int32_t value);
		void push_int64_t(const int64_t value);

		void push_uint8_t(const uint8_t value);
		void push_uint16_t(const uint16_t value);
		void push_uint32_t(const uint32_t value);
		void push_uint64_t(const uint64_t value);

		void push_float(const float value);
		void push_double(const double value);
		void push_ptr(const void* px);

		bool trypush_int8_t(const int8_t value);
		bool trypush_int16_t(const int16_t value);
		bool trypush_int32_t(const int32_t value);
		bool trypush_int64_t(const int64_t value);

		bool trypush_uint8_t(const uint8_t value);
		bool trypush_uint16_t(const uint16_t value);
		bool trypush_uint32_t(const uint32_t value);
		bool trypush_uint64_t(const uint64_t value);

		bool trypush_float(const float value);
		bool trypush_double(const double value);
		bool trypush_ptr(const void* px);

	public: // text output
		std::size_t str_printf(const char* format, ...);

		std::size_t str(const char* format);
		std::size_t str(const int32_t v);
		std::size_t str(const uint32_t v);
	};

	//-----------------------------------------------------------------------------------------------------------

	struct std_file_reader_impl : public std_file_handle
	{
	public:
		std_file_reader_impl() = default;
		std_file_reader_impl(const std_file_reader_impl&) = delete;
		std_file_reader_impl& operator=(const std_file_reader_impl&) = delete;

	protected:
		std_file_reader_impl(std::FILE* handle);

	public:
		bool open(const char* abs_path, const bool binary);
		bool open(const char* abs_path);
		bool open_binary(const char* abs_path);

	public: // binary
		void read_raw_buffer(void* dest, const std::size_t ammount);
		bool tryread_raw_buffer(void* dest, const std::size_t ammount);
	public: // bool 	pop_T(T & out);
		bool pop_uint8_t(uint8_t& out);
		bool pop_uint16_t(uint16_t& out);
		bool pop_uint32_t(uint32_t& out);
		bool pop_uint64_t(uint64_t& out);

		bool pop_int8_t(int8_t& out);
		bool pop_int16_t(int16_t& out);
		bool pop_int32_t(int32_t& out);
		bool pop_int64_t(int64_t& out);

		bool pop_float(float& out);
		bool pop_double(double& out);

	public: // T 	pop_T();
		uint8_t	 pop_uint8_t();
		uint16_t pop_uint16_t();
		uint32_t pop_uint32_t();
		uint64_t pop_uint64_t();

		int8_t	pop_int8_t();
		int16_t pop_int16_t();
		int32_t pop_int32_t();
		int64_t pop_int64_t();

		float  pop_float();
		double pop_double();

	#include "cllio_pop_variants.h"

	public: // extra:
		// read file content int a std::vector like thing of uint8_t
		template <class T>
		bool read_into_container(T& out)
		{
			bool ok;
			auto current_size = out.size();
			auto remaining = get_remaining_size();
			auto elements_to_read = remaining / sizeof(typename T::value_type);
			
			if (remaining % sizeof(typename T::value_type) != 0)
			{
				out.resize(current_size + elements_to_read + 1, 0);
				ok = false;
			}
			else
			{
				out.resize(current_size + elements_to_read);
				ok = true;
			}
			read_raw_buffer(out.data() + current_size, remaining);
			return ok;
		}
	};

	//-----------------------------------------------------------------------------------------------------------

	template <class T>
	struct std_file_io_view : public T
	{
	public:
		using base_t = T;
		using class_t = std_file_io_view<T>;

	public:
		std_file_io_view() = default;
		void close() = delete;
		bool open(const char*, const bool = true, const bool = false) = delete;
		bool open(const char*, const bool) = delete;
		bool open(const char*) = delete;
		bool open_binary(const char*) = delete;

	public:
		inline std_file_io_view(std::FILE* file_view)
			: base_t(file_view)
		{
		}
		inline std_file_io_view(const class_t& other)
			: base_t(other.m_file_ptr)
		{
		}
		inline class_t& operator=(const class_t& other)
		{
			base_t::m_file_ptr = other.m_file_ptr;
			return (*this);
		}
	};
	template <class T>
	struct std_file_io_owner : public T
	{
	public:
		using base_t = T;
		using class_t = std_file_io_owner<T>;

	public:
		std_file_io_owner() = default;
		std_file_io_owner(const class_t& other) = delete;
		class_t& operator=(const class_t& other) = delete;

		inline ~std_file_io_owner()
		{
			base_t::close();
		}
		void swap(class_t& other)
		{
			std::FILE* tmp = base_t::m_file_ptr;
			base_t::m_file_ptr = other.m_file_ptr;
			other.m_file_ptr = tmp;
		}

		inline std_file_io_owner(class_t&& other)
		{
			swap(other);
		}
		inline class_t& operator=(class_t&& other)
		{
			base_t::close();
			swap(other);
			return (*this);
		}
	};

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------

	// non owning
	using std_file_read_view = std_file_io_view<std_file_reader_impl>;
	using std_file_write_view = std_file_io_view<std_file_writer_impl>;

	// owning
	using std_file_read = std_file_io_owner<std_file_reader_impl>;
	using std_file_write = std_file_io_owner<std_file_writer_impl>;

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------


}
