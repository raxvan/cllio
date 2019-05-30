#pragma once
#include "cllio_utils.h"

namespace cllio
{

	struct mem_stream_read_unchecked
	{
	protected:
		const byte_t* m_px = nullptr;

	public:
		mem_stream_read_unchecked(const void * px);
		
		mem_stream_read_unchecked() = default;
		mem_stream_read_unchecked(const mem_stream_read_unchecked &) = default;
		mem_stream_read_unchecked& operator = (const mem_stream_read_unchecked &) = default;
	public:
		inline bool isValid() const
		{
			return m_px != nullptr;
		}
		inline const byte_t* begin() const
		{
			return m_px;
		}
	public:
		void		buffer_read(void * dest, const std::size_t ammount);
	public:
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
	};

	//-----------------------------------------------------------------------------------------------------------

	struct mem_stream_read
	{
	protected:
		//remaining size in bytes
		const byte_t* m_px = nullptr;
		const byte_t* m_px_end = nullptr;
	public:
		mem_stream_read(const void * px, const std::size_t size_in_bytes);
		
		mem_stream_read() = default;
		mem_stream_read(const mem_stream_read &) = default;
		mem_stream_read& operator = (const mem_stream_read &) = default;

		//no size_t because of implementatin defined https://en.cppreference.com/w/cpp/types/ptrdiff_t
		std::ptrdiff_t size() const;
	public:
		inline bool isValid() const
		{
			return m_px != nullptr;
		}
		inline const uint8_t* begin() const
		{
			return m_px;
		}
		inline const uint8_t* end() const
		{
			return m_px_end;
		}
	public: //binary
		void		buffer_read(void * dest, const std::size_t ammount);

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
	public: //T 	popdefault_T(T& out, const T & default);
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

	struct mem_stream_write_unchecked
	{
	protected:
		byte_t* m_px = nullptr;

	public:
		mem_stream_write_unchecked(void * px);
		
		mem_stream_write_unchecked() = default;
		mem_stream_write_unchecked(const mem_stream_write_unchecked &) = default;
		mem_stream_write_unchecked& operator = (const mem_stream_write_unchecked &) = default;

	public:
		inline byte_t* data()const
		{
			return m_px;
		}
		inline bool isValid() const
		{
			return m_px != nullptr;
		}
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
	};


	//-----------------------------------------------------------------------------------------------------------

	struct mem_stream_write
	{
	protected:
		byte_t* m_px = nullptr;
		const byte_t* m_px_end = nullptr;

	public:
		mem_stream_write(void * px,const std::size_t size_in_bytes);
		
		mem_stream_write() = default;
		mem_stream_write(const mem_stream_write &) = default;
		mem_stream_write& operator = (const mem_stream_write &) = default;
	public:
		inline bool isValid() const
		{
			return m_px != nullptr;
		}
		inline const uint8_t* begin() const
		{
			return m_px;
		}
		inline const uint8_t* end() const
		{
			return m_px_end;
		}

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
	};
}