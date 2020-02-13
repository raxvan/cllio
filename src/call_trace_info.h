#pragma once
#include "cllio_utils.h"

namespace cllio
{

	struct static_string
	{
		const char* 		str;
		const std::size_t 	size;
	};

	// debug utils:
	// requires compatible function `void operator()(const std::size_t _total_size,const std::size_t _local_size, const static_string& _fs)`
	// where:
	// _total_size is the ammount of bytes pushed
	// _local_size is the ammount of bytes in this call
	// _fs name of the function called

	using trace_function_t = void(const std::size_t,const std::size_t, const static_string& s);

	template <class F>
	struct call_trace_info : public F
	{
	protected:
		std::size_t m_size = 0;
	protected:
		template <class T,std::size_t N>
		inline void _trace(const char (&c_str)[N])
		{
			(*static_cast<F*>(this))(m_size,sizeof(T),static_string{c_str,N});
			m_size += sizeof(T);
		}
		template <std::size_t N>
		inline void _trace(const std::size_t sz,const char (&c_str)[N])
		{
			(*static_cast<F*>(this))(m_size,sz,static_string{c_str,N});
			m_size += sz;
		}
	public:
		call_trace_info() = default;
		call_trace_info(const call_trace_info<F>&) = default;
		call_trace_info<F>& operator =(const call_trace_info<F>&) = default;

		call_trace_info(const F& _func)
			: F(_func)
		{
		}
	public:
		template <std::size_t N>
		inline void log(const char (&c_str)[N])
		{
			(*static_cast<F*>(this))(m_size,0,static_string{c_str,N});
		}
	public:
		inline void push_raw_buffer(const void*, const std::size_t byte_count)
		{
			_trace(byte_count,"push_raw_buffer");
		}
		inline bool trypush_raw_buffer(const void *, const std::size_t byte_count)
		{
			_trace(byte_count,"trypush_raw_buffer");
			return true;
		}

	public:
		inline void push_int8_t(const int8_t)
		{
			_trace<int8_t>("push_int8_t");
		}
		inline void push_int16_t(const int16_t)
		{
			_trace<int16_t>("push_int16_t");
		}
		inline void push_int32_t(const int32_t)
		{
			_trace<int32_t>("push_int32_t");
		}
		inline void push_int64_t(const int64_t)
		{
			_trace<int64_t>("push_int64_t");
		}

		inline void push_uint8_t(const uint8_t)
		{
			_trace<uint8_t>("push_uint8_t");
		}
		inline void push_uint16_t(const uint16_t)
		{
			_trace<uint16_t>("push_uint16_t");
		}
		inline void push_uint32_t(const uint32_t)
		{
			_trace<uint32_t>("push_uint32_t");
		}
		inline void push_uint64_t(const uint64_t)
		{
			_trace<uint64_t>("push_uint64_t");
		}

		inline void push_float(const float)
		{
			_trace<uint32_t>("push_float");
		}
		inline void push_double(const double)
		{
			_trace<uint64_t>("push_double");
		}
	public:
		inline void trypush_int8_t(const int8_t)
		{
			_trace<int8_t>("trypush_int8_t");
		}
		inline void trypush_int16_t(const int16_t)
		{
			_trace<int16_t>("trypush_int16_t");
		}
		inline void trypush_int32_t(const int32_t)
		{
			_trace<int32_t>("trypush_int32_t");
		}
		inline void trypush_int64_t(const int64_t)
		{
			_trace<int64_t>("trypush_int64_t");
		}

		inline void trypush_uint8_t(const uint8_t)
		{
			_trace<uint8_t>("trypush_uint8_t");
		}
		inline void trypush_uint16_t(const uint16_t)
		{
			_trace<uint16_t>("trypush_uint16_t");
		}
		inline void trypush_uint32_t(const uint32_t)
		{
			_trace<uint32_t>("trypush_uint32_t");
		}
		inline void trypush_uint64_t(const uint64_t)
		{
			_trace<uint64_t>("trypush_uint64_t");
		}

		inline void trypush_float(const float)
		{
			_trace<uint32_t>("trypush_float");
		}
		inline void trypush_double(const double)
		{
			_trace<uint64_t>("trypush_double");
		}
	};

	template <class F>
	call_trace_info<F> make_call_tracer(const F& _func)
	{
		return call_trace_info<F>(_func);
	}
}
