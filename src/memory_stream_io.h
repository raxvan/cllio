#pragma once
#include "cllio_utils.h"

namespace cllio
{
	// fwd declare:

	struct mem_stream_read;
	struct mem_stream_read_unchecked;

	struct mem_stream_write;
	struct mem_stream_write_unchecked;

	template <class F>
	struct memory_functor_write;
	//^ F is required to have `cllio::byte_t* operator()(const std::size_t sz); `

	//-----------------------------------------------------------------------------------------------------------

	//read from raw pointer
	struct mem_stream_read_unchecked
	{
	protected:
		const byte_t* m_px = nullptr;

	public:
		mem_stream_read_unchecked(const void* px);

		mem_stream_read_unchecked() = default;
		mem_stream_read_unchecked(const mem_stream_read_unchecked&) = default;
		mem_stream_read_unchecked& operator=(const mem_stream_read_unchecked&) = default;

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
		void read_raw_buffer(void* dest, const std::size_t ammount);

	public:
		uint8_t  pop_uint8_t();
		uint16_t pop_uint16_t();
		uint32_t pop_uint32_t();
		uint64_t pop_uint64_t();

		int8_t  pop_int8_t();
		int16_t pop_int16_t();
		int32_t pop_int32_t();
		int64_t pop_int64_t();

		float  pop_float();
		double pop_double();
	};

	//-----------------------------------------------------------------------------------------------------------

	//read from raw memory range (given as raw pointer)
	struct mem_stream_read
	{
	protected:
		// remaining size in bytes
		const byte_t* m_px = nullptr;
		const byte_t* m_px_end = nullptr;

	public:
		mem_stream_read(const void* px, const std::size_t size_in_bytes);

		mem_stream_read() = default;
		mem_stream_read(const mem_stream_read&) = default;
		mem_stream_read& operator=(const mem_stream_read&) = default;

		// no size_t because of implementatin defined https://en.cppreference.com/w/cpp/types/ptrdiff_t
		std::ptrdiff_t size() const;

	public:
		inline bool hasData()const
		{
			return m_px < m_px_end;
		}
		inline bool hasData(const std::size_t expected_bytes)const
		{
			return (m_px + expected_bytes) <= m_px_end;
		}
		inline bool isValid() const
		{
			return m_px != nullptr;
		}
		inline void operator += (const std::size_t offset)
		{
			m_px += offset;
		}
	public:
		inline const byte_t* begin() const
		{
			return m_px;
		}
		inline const byte_t* end() const
		{
			return m_px_end;
		}

	public: // binary
		void read_raw_buffer(void* dest, const std::size_t ammount);
		bool tryread_raw_buffer(void* dest, const std::size_t ammount); //returns true if read is successfull

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
		uint8_t  pop_uint8_t();
		uint16_t pop_uint16_t();
		uint32_t pop_uint32_t();
		uint64_t pop_uint64_t();

		int8_t  pop_int8_t();
		int16_t pop_int16_t();
		int32_t pop_int32_t();
		int64_t pop_int64_t();

		float  pop_float();
		double pop_double();

	public: // T 	popdefault_T(const T & default);
		uint8_t  popdefault_uint8_t(const uint8_t _default);
		uint16_t popdefault_uint16_t(const uint16_t _default);
		uint32_t popdefault_uint32_t(const uint32_t _default);
		uint64_t popdefault_uint64_t(const uint64_t _default);

		int8_t  popdefault_int8_t(const int8_t _default);
		int16_t popdefault_int16_t(const int16_t _default);
		int32_t popdefault_int32_t(const int32_t _default);
		int64_t popdefault_int64_t(const int64_t _default);

		float  popdefault_float(const float _default);
		double popdefault_double(const double _default);

	public: // T 	popdefault_T(T& out, const T & default);
		bool popdefault_uint8_t(uint8_t& out, const uint8_t _default);
		bool popdefault_uint16_t(uint16_t& out, const uint16_t _default);
		bool popdefault_uint32_t(uint32_t& out, const uint32_t _default);
		bool popdefault_uint64_t(uint64_t& out, const uint64_t _default);

		bool popdefault_int8_t(int8_t& out, const int8_t _default);
		bool popdefault_int16_t(int16_t& out, const int16_t _default);
		bool popdefault_int32_t(int32_t& out, const int32_t _default);
		bool popdefault_int64_t(int64_t& out, const int64_t _default);

		bool popdefault_float(float& out, const float _default);
		bool popdefault_double(double& out, const double _default);

	public: // void 	 pop_T(T & out, bool& error);
		void pop_uint8_t(uint8_t& out, bool& error);
		void pop_uint16_t(uint16_t& out, bool& error);
		void pop_uint32_t(uint32_t& out, bool& error);
		void pop_uint64_t(uint64_t& out, bool& error);

		void pop_int8_t(int8_t& out, bool& error);
		void pop_int16_t(int16_t& out, bool& error);
		void pop_int32_t(int32_t& out, bool& error);
		void pop_int64_t(int64_t& out, bool& error);

		void pop_float(float& out, bool& error);
		void pop_double(double& out, bool& error);

	public: // T 	 pop_T(bool& error);
		uint8_t  pop_uint8_t(bool& error);
		uint16_t pop_uint16_t(bool& error);
		uint32_t pop_uint32_t(bool& error);
		uint64_t pop_uint64_t(bool& error);

		int8_t  pop_int8_t(bool& error);
		int16_t pop_int16_t(bool& error);
		int32_t pop_int32_t(bool& error);
		int64_t pop_int64_t(bool& error);

		float  pop_float(bool& error);
		double pop_double(bool& error);
	};

	//-----------------------------------------------------------------------------------------------------------

	//write to raw pointer
	struct mem_stream_write_unchecked
	{
	protected:
		byte_t* m_px = nullptr;

	public:
		mem_stream_write_unchecked(void* px);

		mem_stream_write_unchecked() = default;
		mem_stream_write_unchecked(const mem_stream_write_unchecked&) = default;
		mem_stream_write_unchecked& operator=(const mem_stream_write_unchecked&) = default;

	public:
		inline byte_t* data() const
		{
			return m_px;
		}
		inline bool isValid() const
		{
			return m_px != nullptr;
		}
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
	};

	//-----------------------------------------------------------------------------------------------------------

	//write to raw memory range
	struct mem_stream_write
	{
	protected:
		byte_t*		  m_px = nullptr;
		const byte_t* m_px_end = nullptr;

	public:
		mem_stream_write(void* px, const std::size_t size_in_bytes);

		mem_stream_write() = default;
		mem_stream_write(const mem_stream_write&) = default;
		mem_stream_write& operator=(const mem_stream_write&) = default;

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

		template <class T>
		inline bool _can_write() const
		{
			return (m_px != nullptr && (m_px + sizeof(T)) <= m_px_end);
		}
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
	public:
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
	};

	//-----------------------------------------------------------------------------------------------------------

	struct _memory_functor_write_details
	{
		static void _write_bynary_uint16_t(byte_t* out, const uint16_t value);
		static void _write_bynary_uint32_t(byte_t* out, const uint32_t value);
		static void _write_bynary_uint64_t(byte_t* out, const uint64_t value);
		static void _copy_memory(void * _dst, const void* _src, const std::size_t byte_count);
	};

	//-----------------------------------------------------------------------------------------------------------

	//write to raw memory given by a functor
	template <class F>
	struct memory_functor_write : public F
	{
	public:
		using class_t = memory_functor_write<F>;

	public:
		memory_functor_write() = default;
		memory_functor_write(const class_t&) = default;
		class_t& operator=(const class_t&) = default;

		inline memory_functor_write(F&& _func);
		inline memory_functor_write(const F& _func);
		inline memory_functor_write(class_t && other);
		inline class_t & operator = (class_t && other);

	protected:
		template <class T>
		inline byte_t* _get()
		{
			byte_t* out = (*static_cast<F*>(this))(sizeof(T));
			CLLIO_ASSERT(out != nullptr);
			return out;
		}
		template <class T>
		inline byte_t* _tryget()
		{
			byte_t* out = (*static_cast<F*>(this))(sizeof(T));
			return out;
		}

	public:
		inline void push_uint8_t(const uint8_t value);
		inline void push_uint16_t(const uint16_t value);
		inline void push_uint32_t(const uint32_t value);
		inline void push_uint64_t(const uint64_t value);

		inline void push_int8_t(const int8_t value);
		inline void push_int16_t(const int16_t value);
		inline void push_int32_t(const int32_t value);
		inline void push_int64_t(const int64_t value);

		inline void push_float(const float value);
		inline void push_double(const double value);

		inline void push_ptr(const void* px);

	public: //special flavor of push that can fail
		inline bool trypush_uint8_t(const uint8_t value);
		inline bool trypush_uint16_t(const uint16_t value);
		inline bool trypush_uint32_t(const uint32_t value);
		inline bool trypush_uint64_t(const uint64_t value);

		inline bool trypush_int8_t(const int8_t value);
		inline bool trypush_int16_t(const int16_t value);
		inline bool trypush_int32_t(const int32_t value);
		inline bool trypush_int64_t(const int64_t value);

		inline bool trypush_float(const float value);
		inline bool trypush_double(const double value);
		inline bool trypush_ptr(const void* px);

	public: //raw buffer functions
		inline void push_raw_buffer(const void * data, const std::size_t byte_count);
		inline bool trypush_raw_buffer(const void * data, const std::size_t byte_count);
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	template <class V>
	struct vector_append_utility
	{
		V		buffer;
		inline cllio::byte_t* operator()(const std::size_t sz)
		{
			auto index = buffer.size();
			buffer.resize(buffer.size() + sz);
			return &buffer[index];
		}
	};

	template <class V>
	using memory_vector_serializer = memory_functor_write< vector_append_utility<V> >;

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------

	inline void mem_stream_read::pop_uint8_t(uint8_t& out, bool& error) {
		if(error) return;
		error = pop_uint8_t(out) == false;
	}
	inline void mem_stream_read::pop_uint16_t(uint16_t& out, bool& error) {
		if(error) return;
		error = pop_uint16_t(out) == false;
	}
	inline void mem_stream_read::pop_uint32_t(uint32_t& out, bool& error) {
		if(error) return;
		error = pop_uint32_t(out) == false;
	}
	inline void mem_stream_read::pop_uint64_t(uint64_t& out, bool& error) {
		if(error) return;
		error = pop_uint64_t(out) == false;
	}

	inline void mem_stream_read::pop_int8_t(int8_t& out, bool& error) {
		if(error) return;
		error = pop_int8_t(out) == false;
	}
	inline void mem_stream_read::pop_int16_t(int16_t& out, bool& error) {
		if(error) return;
		error = pop_int16_t(out) == false;
	}
	inline void mem_stream_read::pop_int32_t(int32_t& out, bool& error) {
		if(error) return;
		error = pop_int32_t(out) == false;
	}
	inline void mem_stream_read::pop_int64_t(int64_t& out, bool& error) {
		if(error) return;
		error = pop_int64_t(out) == false;
	}

	inline void mem_stream_read::pop_float(float& out, bool& error) {
		if(error) return;
		error = pop_float(out) == false;
	}
	inline void mem_stream_read::pop_double(double& out, bool& error) {
		if(error) return;
		error = pop_double(out) == false;
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F>
	inline memory_functor_write<F>::memory_functor_write(F&& _func)
			: F{std::forward<F>(_func)}
	{
	}
	template <class F>
	inline memory_functor_write<F>::memory_functor_write(const F& _func)
		: F{_func}
	{
	}

	template <class F>
	inline memory_functor_write<F>::memory_functor_write(class_t && other)
		: F{std::forward<F>(other)}
	{
	}
	template <class F>
	inline typename memory_functor_write<F>::class_t& memory_functor_write<F>::operator = (typename memory_functor_write<F>::class_t&& other)
	{
		*static_cast<F*>(this) = std::move(static_cast<F&>(other));
		return (*this);
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F> inline void memory_functor_write<F>::push_uint8_t(const uint8_t value)
	{
		byte_t* out = _get<uint8_t>();
		*out = value;
	}
	template <class F> inline void memory_functor_write<F>::push_uint16_t(const uint16_t value)
	{
		byte_t* out = _get<uint16_t>();
		_memory_functor_write_details::_write_bynary_uint16_t(out, value);
	}
	template <class F> inline void memory_functor_write<F>::push_uint32_t(const uint32_t value)
	{
		byte_t* out = _get<uint32_t>();
		_memory_functor_write_details::_write_bynary_uint32_t(out, value);
	}
	template <class F> inline void memory_functor_write<F>::push_uint64_t(const uint64_t value)
	{
		byte_t* out = _get<uint64_t>();
		_memory_functor_write_details::_write_bynary_uint64_t(out, value);
	}

	template <class F> inline void memory_functor_write<F>::push_int8_t(const int8_t value)
	{
		byte_t*					   out = _get<int8_t>();
		UnionCast<int8_t, uint8_t> tmp;
		tmp.first = value;
		*out = tmp.second;
	}
	template <class F> inline void memory_functor_write<F>::push_int16_t(const int16_t value)
	{
		byte_t*						 out = _get<int16_t>();
		UnionCast<int16_t, uint16_t> tmp;
		tmp.first = value;
		_memory_functor_write_details::_write_bynary_uint16_t(out, tmp.second);
	}
	template <class F> inline void memory_functor_write<F>::push_int32_t(const int32_t value)
	{
		byte_t*						 out = _get<int32_t>();
		UnionCast<int32_t, uint32_t> tmp;
		tmp.first = value;
		_memory_functor_write_details::_write_bynary_uint32_t(out, tmp.second);
	}
	template <class F> inline void memory_functor_write<F>::push_int64_t(const int64_t value)
	{
		byte_t*						 out = _get<int64_t>();
		UnionCast<int64_t, uint64_t> tmp;
		tmp.first = value;
		_memory_functor_write_details::_write_bynary_uint64_t(out, tmp.second);
	}

	template <class F> inline void memory_functor_write<F>::push_float(const float value)
	{
		byte_t*					   out = _get<float>();
		UnionCast<float, uint32_t> tmp;
		tmp.first = value;
		_memory_functor_write_details::_write_bynary_uint32_t(out, tmp.second);
	}
	template <class F> inline void memory_functor_write<F>::push_double(const double value)
	{
		byte_t*						out = _get<double>();
		UnionCast<double, uint64_t> tmp;
		tmp.first = value;
		_memory_functor_write_details::_write_bynary_uint64_t(out, tmp.second);
	}
	template <class F> inline void memory_functor_write<F>::push_ptr(const void* px)
	{
		byte_t* out = _get<uint64_t>();
		UnionCast<uint64_t,const void *> tmp;
		tmp.first = 0;
		tmp.second = px;
		_memory_functor_write_details::_write_bynary_uint64_t(out, tmp.first);
	}

	inline void push_ptr();
	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F> inline bool memory_functor_write<F>::trypush_uint8_t(const uint8_t value)
	{
		byte_t* out = _tryget<uint8_t>();
		if(out != nullptr)
			*out = value;
		return out != nullptr;
	}
	template <class F> inline bool memory_functor_write<F>::trypush_uint16_t(const uint16_t value)
	{
		byte_t* out = _tryget<uint16_t>();
		if(out != nullptr)
			_memory_functor_write_details::_write_bynary_uint16_t(out, value);
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_uint32_t(const uint32_t value)
	{
		byte_t* out = _tryget<uint32_t>();
		if(out != nullptr)
			_memory_functor_write_details::_write_bynary_uint32_t(out, value);
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_uint64_t(const uint64_t value)
	{
		byte_t* out = _tryget<uint64_t>();
		if(out != nullptr)
			_memory_functor_write_details::_write_bynary_uint64_t(out, value);
		return (out != nullptr);
	}

	template <class F> inline bool memory_functor_write<F>::trypush_int8_t(const int8_t value)
	{
		byte_t*					   out = _tryget<int8_t>();
		if (out != nullptr)
		{
			UnionCast<int8_t, uint8_t> tmp;
			tmp.first = value;
			*out = tmp.second;
		}
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_int16_t(const int16_t value)
	{
		byte_t*						 out = _tryget<int16_t>();
		if (out != nullptr)
		{
			UnionCast<int16_t, uint16_t> tmp;
			tmp.first = value;
			_memory_functor_write_details::_write_bynary_uint16_t(out, tmp.second);
		}
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_int32_t(const int32_t value)
	{
		byte_t*						 out = _tryget<int32_t>();
		if (out != nullptr)
		{
			UnionCast<int32_t, uint32_t> tmp;
			tmp.first = value;
			_memory_functor_write_details::_write_bynary_uint32_t(out, tmp.second);
		}
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_int64_t(const int64_t value)
	{
		byte_t*						 out = _tryget<int64_t>();
		if (out != nullptr)
		{
			UnionCast<int64_t, uint64_t> tmp;
			tmp.first = value;
			_memory_functor_write_details::_write_bynary_uint64_t(out, tmp.second);
		}
		return (out != nullptr);
	}

	template <class F> inline bool memory_functor_write<F>::trypush_float(const float value)
	{
		byte_t*					   out = _tryget<float>();
		if (out != nullptr)
		{
			UnionCast<float, uint32_t> tmp;
			tmp.first = value;
			_memory_functor_write_details::_write_bynary_uint32_t(out, tmp.second);
		}
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_double(const double value)
	{
		byte_t*						out = _tryget<double>();
		if (out != nullptr)
		{
			UnionCast<double, uint64_t> tmp;
			tmp.first = value;
			_memory_functor_write_details::_write_bynary_uint64_t(out, tmp.second);
		}
		return (out != nullptr);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_ptr(const void* px)
	{
		byte_t* out = _tryget<uint64_t>();
		if (out != nullptr)
		{
			UnionCast<uint64_t,const void*> tmp;
			tmp.first = 0;
			tmp.second = px;
			_memory_functor_write_details::_write_bynary_uint64_t(out, tmp.first);
		}
		return (out != nullptr);
	}

	template <class F> inline void memory_functor_write<F>::push_raw_buffer(const void * data, const std::size_t byte_count)
	{
		byte_t* out = (*static_cast<F*>(this))(byte_count);
		_memory_functor_write_details::_copy_memory(out, data, byte_count);
	}
	template <class F> inline bool memory_functor_write<F>::trypush_raw_buffer(const void * data, const std::size_t byte_count)
	{
		byte_t* out = (*static_cast<F*>(this))(byte_count);
		if (out != nullptr)
			_memory_functor_write_details::_copy_memory(out, data, byte_count);
		return (out != nullptr);
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	template <class F>
	inline memory_functor_write<F> make_memory_writer(const F& func)
	{
		return memory_functor_write<F>{func};
	}
	template <class F>
	inline memory_functor_write<F> make_memory_writer(F&& func)
	{
		return memory_functor_write<F>{std::forward<F>(func)};
	}
	//--------------------------------------------------------------------------------------------------------------------------------

}
