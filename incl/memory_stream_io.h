#pragma once
#include "cllio_internal_utils.h"

namespace cllio
{
	// fwd declare:

	struct memory_rstream;
	struct memory_rstream_unchecked;

	struct memory_wstream;
	struct memory_wstream_unchecked;

	template <class F>
	struct memory_wfunc;
	//^ F is required to have `cllio::byte_t* operator()(const std::size_t size_in_bytes); `

	template <class F>
	struct memory_rfunc;
	//^ F is required to have `const cllio::byte_t* operator()(const std::size_t size_in_bytes); `
	//                   and  `bool operator()(cllio::byte_t* out, const std::size_t size_in_bytes); `

	//-----------------------------------------------------------------------------------------------------------

	// read from raw pointer
	struct memory_rstream_unchecked
	{
	protected:
		const byte_t* m_px = nullptr;

	public:
		memory_rstream_unchecked(const void* px);

		memory_rstream_unchecked() = default;
		memory_rstream_unchecked(const memory_rstream_unchecked&) = default;
		memory_rstream_unchecked& operator=(const memory_rstream_unchecked&) = default;

	public:
		inline bool valid() const
		{
			return m_px != nullptr;
		}
		inline const byte_t* begin() const
		{
			return m_px;
		}

	public:
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
	};

	//-----------------------------------------------------------------------------------------------------------

	// read from raw memory range (given as raw pointer)
	struct memory_rstream
	{
	protected:
		// remaining size in bytes
		const byte_t* m_px = nullptr;
		const byte_t* m_px_end = nullptr;

	public:
		memory_rstream(const void* px, const std::size_t size_in_bytes);

		memory_rstream() = default;
		memory_rstream(const memory_rstream&) = default;
		memory_rstream& operator=(const memory_rstream&) = default;

		// no size_t because of implementatin defined https://en.cppreference.com/w/cpp/types/ptrdiff_t
		std::ptrdiff_t size() const;

	public:
		inline bool hasData() const
		{
			return m_px < m_px_end;
		}
		inline bool hasData(const std::size_t expected_bytes) const
		{
			return (m_px + expected_bytes) <= m_px_end;
		}
		inline bool valid() const
		{
			return m_px != nullptr;
		}
		inline void operator+=(const std::size_t offset)
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
		inline const byte_t* data() const
		{
			return m_px;
		}

		template <class T>
		inline bool _can_read() const
		{
			return (m_px + sizeof(T)) <= m_px_end;
		}

	public: // binary
		void read_raw_buffer(void* dest, const std::size_t ammount);
		bool tryread_raw_buffer(void* dest, const std::size_t ammount); // returns true if read is successfull

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
	};

	//-----------------------------------------------------------------------------------------------------------

	struct memory_wstream_iterator
	{
	protected:
		byte_t* m_px = nullptr;

	public:
		inline byte_t* data() const
		{
			return m_px;
		}
		inline bool valid() const
		{
			return m_px != nullptr;
		}
		inline const byte_t* begin() const
		{
			return m_px;
		}

	public:
		memory_wstream_iterator() = default;
		memory_wstream_iterator(void* px);
	};

	// write to raw pointer
	struct memory_wstream_unchecked : public memory_wstream_iterator
	{
	public:
		using memory_wstream_iterator::memory_wstream_iterator;

		memory_wstream_unchecked() = default;
		memory_wstream_unchecked(const memory_wstream_unchecked&) = default;
		memory_wstream_unchecked& operator=(const memory_wstream_unchecked&) = default;

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

	// write to raw memory range
	struct memory_wstream : public memory_wstream_iterator
	{
	protected:
		const byte_t* m_px_end = nullptr;

	public:
		memory_wstream(void* px, const std::size_t size_in_bytes);

		memory_wstream() = default;
		memory_wstream(const memory_wstream&) = default;
		memory_wstream& operator=(const memory_wstream&) = default;

	public:
		inline const byte_t* end() const
		{
			return m_px_end;
		}

	protected:
		template <class T>
		inline bool _can_write() const
		{
			return (m_px + sizeof(T)) <= m_px_end;
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

	public:
		void write_raw_buffer(const void* data, const std::size_t byte_count);
		bool trywrite_raw_buffer(const void* data, const std::size_t byte_count);
	};

	//-----------------------------------------------------------------------------------------------------------

	// write to raw memory given by a functor
	template <class F>
	struct memory_wfunc : public F
	{
	public:
		using class_t = memory_wfunc<F>;

	public:
		memory_wfunc() = default;
		memory_wfunc(const class_t&) = default;
		class_t& operator=(const class_t&) = default;

		inline memory_wfunc(F&& _func);
		inline memory_wfunc(const F& _func);
		inline memory_wfunc(class_t&& other);
		inline class_t& operator=(class_t&& other);

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

	public: // special flavor of push that can fail
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

	public: // raw buffer functions
		inline void write_raw_buffer(const void* data, const std::size_t byte_count);
		inline bool trywrite_raw_buffer(const void* data, const std::size_t byte_count);
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	template <class V>
	struct vector_append_utility
	{
		using buffer_ptr_t = cllio::byte_t*;
		V					buffer;
		inline buffer_ptr_t operator()(const std::size_t sz)
		{
			auto index = buffer.size();
			buffer.resize(buffer.size() + sz);
			return buffer_ptr_t(&buffer[index]);
		}

		inline void clear()
		{
			buffer.clear();
		}

		template <class T>
		inline T& alloc()
		{
			auto index = buffer.size();
			buffer.resize(buffer.size() + sizeof(T));
			return *static_cast<T*>(&buffer[index]);
		}
	};

	template <class V>
	using memory_vector_serializer = memory_wfunc<vector_append_utility<V>>;

	//--------------------------------------------------------------------------------------------------------------------------------

	// read from raw memory range (given as raw pointer)
	template <class F>
	struct memory_rfunc : public F
	{
	protected:
		using class_t = memory_rfunc<F>;

	public:
		memory_rfunc() = default;
		memory_rfunc(const memory_rfunc&) = default;
		memory_rfunc& operator=(const memory_rfunc&) = default;

		inline memory_rfunc(F&& _func);
		inline memory_rfunc(const F& _func);
		inline memory_rfunc(class_t&& other);
		inline class_t& operator=(class_t&& other);

	protected:
		template <class T>
		inline const byte_t* _get()
		{
			const byte_t* out = (*static_cast<F*>(this))(sizeof(T));
			CLLIO_ASSERT(out != nullptr);
			return out;
		}
		template <class T>
		inline const byte_t* _tryget()
		{
			const byte_t* out = (*static_cast<F*>(this))(sizeof(T));
			return out;
		}

	public: // binary
		inline void read_raw_buffer(void* dest, const std::size_t ammount);
		inline bool tryread_raw_buffer(void* dest, const std::size_t ammount); // returns true if read is successfull

	public: // bool 	pop_T(T & out);
		inline bool pop_uint8_t(uint8_t& out);
		inline bool pop_uint16_t(uint16_t& out);
		inline bool pop_uint32_t(uint32_t& out);
		inline bool pop_uint64_t(uint64_t& out);

		inline bool pop_int8_t(int8_t& out);
		inline bool pop_int16_t(int16_t& out);
		inline bool pop_int32_t(int32_t& out);
		inline bool pop_int64_t(int64_t& out);

		inline bool pop_float(float& out);
		inline bool pop_double(double& out);

	public: // T 	pop_T();
		inline uint8_t	pop_uint8_t();
		inline uint16_t pop_uint16_t();
		inline uint32_t pop_uint32_t();
		inline uint64_t pop_uint64_t();

		inline int8_t  pop_int8_t();
		inline int16_t pop_int16_t();
		inline int32_t pop_int32_t();
		inline int64_t pop_int64_t();

		inline float  pop_float();
		inline double pop_double();

#include "cllio_pop_variants.h"
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F>
	inline memory_wfunc<F>::memory_wfunc(F&& _func)
		: F { std::forward<F>(_func) }
	{
	}
	template <class F>
	inline memory_wfunc<F>::memory_wfunc(const F& _func)
		: F { _func }
	{
	}

	template <class F>
	inline memory_wfunc<F>::memory_wfunc(class_t&& other)
		: F { std::forward<F>(other) }
	{
	}
	template <class F>
	inline typename memory_wfunc<F>::class_t& memory_wfunc<F>::operator=(typename memory_wfunc<F>::class_t&& other)
	{
		*static_cast<F*>(this) = std::move(static_cast<F&>(other));
		return (*this);
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F>
	inline memory_rfunc<F>::memory_rfunc(F&& _func)
		: F { std::forward<F>(_func) }
	{
	}
	template <class F>
	inline memory_rfunc<F>::memory_rfunc(const F& _func)
		: F { _func }
	{
	}
	template <class F>
	inline memory_rfunc<F>::memory_rfunc(class_t&& other)
		: F { std::forward<F>(other) }
	{
	}
	template <class F>
	inline memory_rfunc<F>& memory_rfunc<F>::operator=(class_t&& other)
	{
		*static_cast<F*>(this) = std::move(static_cast<F&>(other));
		return (*this);
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F>
	inline void memory_wfunc<F>::push_uint8_t(const uint8_t value)
	{
		byte_t* out = _get<uint8_t>();
		*out = value;
	}
	template <class F>
	inline void memory_wfunc<F>::push_uint16_t(const uint16_t value)
	{
		byte_t* out = _get<uint16_t>();
		_serializer_utils::_write_bynary_uint16_t(out, value);
	}
	template <class F>
	inline void memory_wfunc<F>::push_uint32_t(const uint32_t value)
	{
		byte_t* out = _get<uint32_t>();
		_serializer_utils::_write_bynary_uint32_t(out, value);
	}
	template <class F>
	inline void memory_wfunc<F>::push_uint64_t(const uint64_t value)
	{
		byte_t* out = _get<uint64_t>();
		_serializer_utils::_write_bynary_uint64_t(out, value);
	}

	template <class F>
	inline void memory_wfunc<F>::push_int8_t(const int8_t value)
	{
		byte_t* out = _get<uint8_t>();
		*out = _serializer_utils::_stu8(value);
	}
	template <class F>
	inline void memory_wfunc<F>::push_int16_t(const int16_t value)
	{
		byte_t* out = _get<uint16_t>();
		_serializer_utils::_write_bynary_uint16_t(out, _serializer_utils::_stu16(value));
	}
	template <class F>
	inline void memory_wfunc<F>::push_int32_t(const int32_t value)
	{
		byte_t* out = _get<uint32_t>();
		_serializer_utils::_write_bynary_uint32_t(out, _serializer_utils::_stu32(value));
	}
	template <class F>
	inline void memory_wfunc<F>::push_int64_t(const int64_t value)
	{
		byte_t* out = _get<uint64_t>();
		_serializer_utils::_write_bynary_uint64_t(out, _serializer_utils::_stu64(value));
	}

	template <class F>
	inline void memory_wfunc<F>::push_float(const float value)
	{
		byte_t* out = _get<float>();
		_serializer_utils::_write_bynary_uint32_t(out, _serializer_utils::_ftu(value));
	}
	template <class F>
	inline void memory_wfunc<F>::push_double(const double value)
	{
		byte_t* out = _get<double>();
		_serializer_utils::_write_bynary_uint64_t(out, _serializer_utils::_dtu(value));
	}
	template <class F>
	inline void memory_wfunc<F>::push_ptr(const void* px)
	{
		byte_t* out = _get<uint64_t>();
		_serializer_utils::_write_bynary_uint64_t(out, _serializer_utils::_ptu(px));
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F>
	inline bool memory_wfunc<F>::trypush_uint8_t(const uint8_t value)
	{
		byte_t* out = _tryget<uint8_t>();
		if (out != nullptr)
			*out = value;
		return out != nullptr;
	}
	template <class F>
	inline bool memory_wfunc<F>::trypush_uint16_t(const uint16_t value)
	{
		byte_t* out = _tryget<uint16_t>();
		if (out != nullptr)
			_serializer_utils::_write_bynary_uint16_t(out, value);
		return (out != nullptr);
	}
	template <class F>
	inline bool memory_wfunc<F>::trypush_uint32_t(const uint32_t value)
	{
		byte_t* out = _tryget<uint32_t>();
		if (out != nullptr)
			_serializer_utils::_write_bynary_uint32_t(out, value);
		return (out != nullptr);
	}
	template <class F>
	inline bool memory_wfunc<F>::trypush_uint64_t(const uint64_t value)
	{
		byte_t* out = _tryget<uint64_t>();
		if (out != nullptr)
			_serializer_utils::_write_bynary_uint64_t(out, value);
		return (out != nullptr);
	}

	template <class F>
	inline bool memory_wfunc<F>::trypush_int8_t(const int8_t value)
	{
		byte_t* out = _tryget<uint8_t>();
		if (out != nullptr)
		{
			*out = _serializer_utils::_stu8(value);
		}
		return (out != nullptr);
	}
	template <class F>
	inline bool memory_wfunc<F>::trypush_int16_t(const int16_t value)
	{
		byte_t* out = _tryget<uint16_t>();
		if (out != nullptr)
		{
			_serializer_utils::_write_bynary_uint16_t(out, _serializer_utils::_stu16(value));
		}
		return (out != nullptr);
	}
	template <class F>
	inline bool memory_wfunc<F>::trypush_int32_t(const int32_t value)
	{
		byte_t* out = _tryget<uint32_t>();
		if (out != nullptr)
		{
			_serializer_utils::_write_bynary_uint32_t(out, _serializer_utils::_stu32(value));
		}
		return (out != nullptr);
	}

	template <class F>
	inline bool memory_wfunc<F>::trypush_int64_t(const int64_t value)
	{
		byte_t* out = _tryget<uint64_t>();
		if (out != nullptr)
		{
			_serializer_utils::_write_bynary_uint64_t(out, _serializer_utils::_stu64(value));
		}
		return (out != nullptr);
	}

	template <class F>
	inline bool memory_wfunc<F>::trypush_float(const float value)
	{
		byte_t* out = _tryget<uint32_t>();
		if (out != nullptr)
		{
			_serializer_utils::_write_bynary_uint32_t(out, _serializer_utils::_ftu(value));
		}
		return (out != nullptr);
	}

	template <class F>
	inline bool memory_wfunc<F>::trypush_double(const double value)
	{
		byte_t* out = _tryget<uint32_t>();
		if (out != nullptr)
		{
			_serializer_utils::_write_bynary_uint64_t(out, _serializer_utils::_dtu(value));
		}
		return (out != nullptr);
	}

	template <class F>
	inline bool memory_wfunc<F>::trypush_ptr(const void* px)
	{
		byte_t* out = _tryget<uint64_t>();
		if (out != nullptr)
		{
			_serializer_utils::_write_bynary_uint64_t(out, _serializer_utils::_ptu(px));
		}
		return (out != nullptr);
	}

	template <class F>
	inline void memory_wfunc<F>::write_raw_buffer(const void* data, const std::size_t byte_count)
	{
		byte_t* out = (*static_cast<F*>(this))(byte_count);
		_serializer_utils::_copy_memory(out, data, byte_count);
	}
	template <class F>
	inline bool memory_wfunc<F>::trywrite_raw_buffer(const void* data, const std::size_t byte_count)
	{
		byte_t* out = (*static_cast<F*>(this))(byte_count);
		if (out != nullptr)
			_serializer_utils::_copy_memory(out, data, byte_count);
		return (out != nullptr);
	}
	//--------------------------------------------------------------------------------------------------------------------------------

	template <class F>
	inline void memory_rfunc<F>::read_raw_buffer(void* dest, const std::size_t ammount)
	{
		bool r = (*static_cast<F*>(this))(dest, ammount);
		CLLIO_ASSERT(r == true);
	}
	template <class F>
	inline bool memory_rfunc<F>::tryread_raw_buffer(void* dest, const std::size_t ammount)
	{
		return (*static_cast<F*>(this))(dest, ammount);
	}

	template <class F>
	inline bool memory_rfunc<F>::pop_uint8_t(uint8_t& out)
	{
		const auto* p = _tryget<uint8_t>();
		if (p != nullptr)
			out = *p;
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_uint16_t(uint16_t& out)
	{
		const auto* p = _tryget<uint16_t>();
		if (p != nullptr)
			out = _serializer_utils::_read_uint16_t(p);
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_uint32_t(uint32_t& out)
	{
		const auto* p = _tryget<uint32_t>();
		if (p != nullptr)
			out = _serializer_utils::_read_uint32_t(p);
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_uint64_t(uint64_t& out)
	{
		const auto* p = _tryget<uint64_t>();
		if (p != nullptr)
			out = _serializer_utils::_read_uint64_t(p);
		return p != nullptr;
	}

	template <class F>
	inline bool memory_rfunc<F>::pop_int8_t(int8_t& out)
	{
		const auto* p = _tryget<uint8_t>();
		if (p != nullptr)
			out = _serializer_utils::_uts8(*p);
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_int16_t(int16_t& out)
	{
		const auto* p = _tryget<uint16_t>();
		if (p != nullptr)
			out = _serializer_utils::_uts16(_serializer_utils::_read_uint16_t(p));
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_int32_t(int32_t& out)
	{
		const auto* p = _tryget<uint32_t>();
		if (p != nullptr)
			out = _serializer_utils::_uts32(_serializer_utils::_read_uint32_t(p));
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_int64_t(int64_t& out)
	{
		const auto* p = _tryget<uint64_t>();
		if (p != nullptr)
			out = _serializer_utils::_uts64(_serializer_utils::_read_uint64_t(p));
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_float(float& out)
	{
		const auto* p = _tryget<uint32_t>();
		if (p != nullptr)
			out = _serializer_utils::_utf(_serializer_utils::_read_uint32_t(p));
		return p != nullptr;
	}
	template <class F>
	inline bool memory_rfunc<F>::pop_double(double& out)
	{
		const auto* p = _tryget<uint64_t>();
		if (p != nullptr)
			out = _serializer_utils::_utd(_serializer_utils::_read_uint64_t(p));
		return p != nullptr;
	}

	template <class F>
	inline uint8_t memory_rfunc<F>::pop_uint8_t()
	{
		return *_get<uint8_t>();
	}
	template <class F>
	inline uint16_t memory_rfunc<F>::pop_uint16_t()
	{
		return _serializer_utils::_read_uint16_t(_get<uint16_t>());
	}
	template <class F>
	inline uint32_t memory_rfunc<F>::pop_uint32_t()
	{
		return _serializer_utils::_read_uint32_t(_get<uint32_t>());
	}
	template <class F>
	inline uint64_t memory_rfunc<F>::pop_uint64_t()
	{
		return _serializer_utils::_read_uint64_t(_get<uint64_t>());
	}
	template <class F>
	inline int8_t memory_rfunc<F>::pop_int8_t()
	{
		return _serializer_utils::_uts8(*_get<uint8_t>());
	}
	template <class F>
	inline int16_t memory_rfunc<F>::pop_int16_t()
	{
		return _serializer_utils::_uts16(_serializer_utils::_read_uint16_t(_get<uint16_t>()));
	}
	template <class F>
	inline int32_t memory_rfunc<F>::pop_int32_t()
	{
		return _serializer_utils::_uts32(_serializer_utils::_read_uint32_t(_get<uint32_t>()));
	}
	template <class F>
	inline int64_t memory_rfunc<F>::pop_int64_t()
	{
		return _serializer_utils::_uts64(_serializer_utils::_read_uint64_t(_get<uint64_t>()));
	}
	template <class F>
	inline float memory_rfunc<F>::pop_float()
	{
		return _serializer_utils::_utf(_serializer_utils::_read_uint32_t(_get<uint32_t>()));
	}
	template <class F>
	inline double memory_rfunc<F>::pop_double()
	{
		return _serializer_utils::_utd(_serializer_utils::_read_uint64_t(_get<uint64_t>()));
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	template <class F>
	inline memory_wfunc<F> make_memory_writer(const F& func)
	{
		return memory_wfunc<F> { func };
	}
	template <class F>
	inline memory_wfunc<F> make_memory_writer(F&& func)
	{
		return memory_wfunc<F> { std::forward<F>(func) };
	}
	//--------------------------------------------------------------------------------------------------------------------------------

}
