#pragma once
#include "cllio_utils.h"

namespace cllio
{
	//fwd declare:

	struct mem_stream_read;
	struct mem_stream_read_unchecked;

	struct mem_stream_write;
	struct mem_stream_write_unchecked;

	template <class F>
	struct memory_functor_write;
		//^ F is required to have `cllio::byte_t* operator()(const std::size_t sz); `



	//-----------------------------------------------------------------------------------------------------------

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

	//-----------------------------------------------------------------------------------------------------------


	

	//-----------------------------------------------------------------------------------------------------------

	template <class F>
	struct memory_functor_write
	{
	public:
		using class_t = memory_functor_write<F>;

	public:
		memory_functor_write() = default;
		memory_functor_write(const class_t&) = default;
		class_t& operator = (const class_t&) = default;

		inline memory_functor_write(F && _func)
			:func(std::forward<F>(_func))
		{
		}

	public:
		F 			func;

	protected:
		template <class T>
		inline byte_t*  _get()
		{
			byte_t* out = func(sizeof(T));
			CLLIO_ASSERT(out != nullptr);
			return out;
		}
		inline static void _write_bynary_uint16_t(byte_t* out, const uint16_t value)
		{
			*out++ = static_cast<uint8_t>(value & 0xFF);
			*out = static_cast<uint8_t>((value >> 8) & 0xFF);
		}
		inline static void _write_bynary_uint32_t(byte_t* out, const uint32_t value)
		{
			*out++ = static_cast<uint8_t>(value & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
			*out = static_cast<uint8_t>((value >> 24) & 0xFF);
		}
		inline static void _write_bynary_uint64_t(byte_t* out, const uint64_t value)
		{
			*out++ = static_cast<uint8_t>(value & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 8) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 16) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 24) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 32) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 40) & 0xFF);
			*out++ = static_cast<uint8_t>((value >> 48) & 0xFF);
			*out = static_cast<uint8_t>((value >> 56) & 0xFF);
		}
	public:
		inline void		push_uint8_t(const uint8_t value)
		{
			byte_t* out = _get<uint8_t>();
			*out = value;
		}
		inline void		push_uint16_t(const uint16_t value)
		{
			byte_t* out = _get<uint16_t>();
			_write_bynary_uint16_t(out, value);
		}
		inline void		push_uint32_t(const uint32_t value)
		{
			byte_t* out = _get<uint32_t>();
			_write_bynary_uint32_t(out, value);
		}
		inline void		push_uint64_t(const uint64_t value)
		{
			byte_t* out = _get<uint64_t>();
			_write_bynary_uint64_t(out, value);
		}


		inline void		push_int8_t(const int8_t value)
		{
			byte_t* out = _get<int8_t>();
			UnionCast<int8_t,uint8_t> tmp;
			tmp.first = value;
			*out = tmp.second;
		}
		inline void		push_int16_t(const int16_t value)
		{
			byte_t* out = _get<int16_t>();
			UnionCast<int16_t,uint16_t> tmp;
			tmp.first = value;
			_write_bynary_uint16_t(out,tmp.second);
		}
		inline void		push_int32_t(const int32_t value)
		{
			byte_t* out = _get<int32_t>();
			UnionCast<int32_t,uint32_t> tmp;
			tmp.first = value;
			_write_bynary_uint32_t(out,tmp.second);
		}
		inline void		push_int64_t(const int64_t value)
		{
			byte_t* out = _get<int64_t>();
			UnionCast<int64_t,uint64_t> tmp;
			tmp.first = value;
			_write_bynary_uint64_t(out,tmp.second);
		}

		inline void		push_float(const float value)
		{
			byte_t* out = _get<float>();
			UnionCast<float,uint32_t> tmp;
			tmp.first = value;
			_write_bynary_uint32_t(out,tmp.second);
		}
		inline void		push_double(const double value)
		{
			byte_t* out = _get<double>();
			UnionCast<double,uint64_t> tmp;
			tmp.first = value;
			_write_bynary_uint64_t(out,tmp.second);
		}
	};


}