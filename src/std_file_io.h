#pragma once
#include "cllio_config.h"

namespace cllio
{
	//-----------------------------------------------------------------------------------------------------------
	struct std_file_handle
	{
	public:
		std_file_handle() = default;
		~std_file_handle();
	public:
		std_file_handle(const std_file_handle&) = delete;
		std_file_handle& operator = (const std_file_handle &) = delete;

		void close();
	protected:
		std::FILE *		m_file_ptr = nullptr;
	};
	//-----------------------------------------------------------------------------------------------------------
	struct std_file_writer : public std_file_handle
	{
	public:
		std_file_writer() = default;
		std_file_writer(const std_file_writer&) = delete;
		std_file_writer& operator = (const std_file_writer &) = delete;

		std_file_writer(std_file_writer&&);
		std_file_writer& operator = (std_file_writer &&);
	public:
		bool		open(const char * abs_path, const bool binary = true, const bool append = false);
		void		flush();

	public: //binary writers, no error check
		void		buffer_write(const void * data, const std::size_t byte_count);

	public:
		void		push_int8_t(const int8_t value);
		void		push_int16_t(const int16_t value);
		void		push_int32_t(const int32_t value);
		void		push_int64_t(const int64_t value);

		void		push_uint8_t(const uint8_t value);
		void		push_uint16_t(const uint16_t value);
		void		push_uint32_t(const uint32_t value);
		void		push_uint64_t(const uint64_t value);

		void		push_float(const float value);
		void		push_double(const double value);

	public: //text output
		std::size_t		str_printf(const char * format,...);

		std::size_t		str(const char * format);
		std::size_t		str(const int32_t v);
		std::size_t		str(const uint32_t v);
	};

	//-----------------------------------------------------------------------------------------------------------

	struct std_file_reader : public std_file_handle
	{
	public:
		std_file_reader() = default;
		std_file_reader(const std_file_reader&) = delete;
		std_file_reader& operator = (const std_file_reader &) = delete;
		std_file_reader(std_file_reader&&);
		std_file_reader& operator = (std_file_reader &&);
	public:
		std::size_t get_file_size();
		bool		open(const char * abs_path, const bool binary);
		bool		open(const char * abs_path);
		bool		open_binary(const char * abs_path);

	public: //binary
		bool		buffer_read(void * dest, const std::size_t ammount);

	public: //bool 	pop_T(T & out);
		bool		pop_uint8_t(uint8_t & out);
		bool		pop_uint16_t(uint16_t & out);
		bool		pop_uint32_t(uint32_t & out);
		bool		pop_uint64_t(uint64_t & out);

		bool		pop_int8_t(int8_t & out);
		bool		pop_int16_t(int16_t & out);
		bool		pop_int32_t(int32_t & out);
		bool		pop_int64_t(int64_t & out);

		bool		pop_float(float & out);
		bool		pop_double(double & out);
	public: //T 	pop_T();
		uint8_t		pop_uint8_t();
		uint16_t	pop_uint16_t();
		uint32_t	pop_uint32_t();
		uint64_t	pop_uint64_t();

		int8_t		pop_int8_t();
		int16_t		pop_int16_t();
		int32_t		pop_int32_t();
		int64_t		pop_int64_t();

		float		pop_float();
		double		pop_double();
	public: //T 	popdefault_T(const T & default);
		uint8_t		popdefault_uint8_t(const uint8_t _default);
		uint16_t	popdefault_uint16_t(const uint16_t _default);
		uint32_t	popdefault_uint32_t(const uint32_t _default);
		uint64_t	popdefault_uint64_t(const uint64_t _default);

		int8_t		popdefault_int8_t(const int8_t _default);
		int16_t		popdefault_int16_t(const int16_t _default);
		int32_t		popdefault_int32_t(const int32_t _default);
		int64_t		popdefault_int64_t(const int64_t _default);

		float		popdefault_float(const float _default);
		double		popdefault_double(const double _default);
	public: //
		bool		popdefault_uint8_t(uint8_t & out, const uint8_t _default);
		bool		popdefault_uint16_t(uint16_t & out, const uint16_t _default);
		bool		popdefault_uint32_t(uint32_t & out, const uint32_t _default);
		bool		popdefault_uint64_t(uint64_t & out, const uint64_t _default);

		bool		popdefault_int8_t(int8_t & out, const int8_t _default);
		bool		popdefault_int16_t(int16_t & out, const int16_t _default);
		bool		popdefault_int32_t(int32_t & out, const int32_t _default);
		bool		popdefault_int64_t(int64_t & out, const int64_t _default);

		bool		popdefault_float(float & out, const float _default);
		bool		popdefault_double(double & out, const double _default);
	};

	//-----------------------------------------------------------------------------------------------------------


}