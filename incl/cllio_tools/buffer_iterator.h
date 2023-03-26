
#pragma once

#include "cllio_internal_utils.h"

namespace cllio
{

	struct buffer_iterator
	{
	public:
		buffer_iterator() = default;
		inline buffer_iterator(const void* _data, const std::size_t _size)
			:m_itr(reinterpret_cast<const byte_t*>(_data))
			,m_end(reinterpret_cast<const byte_t*>(_data) + _size)
		{}

	public:
		inline bool hasdata() const
		{
			return m_itr < m_end;
		}

		template <class T>
		inline T read()
		{
			CLLIO_ASSERT(m_itr != nullptr && (m_itr + sizeof(T)) <= m_end);
			T data;
			std::memcpy(&data, m_itr, sizeof(T));
			m_itr = m_itr + sizeof(T);
			return data;
		}
		template <class T>
		inline T peek() const
		{
			CLLIO_ASSERT(m_itr != nullptr && (m_itr + sizeof(T)) <= m_end);
			T data;
			std::memcpy(&data, m_itr, sizeof(T));
			return data;
		}

		template <class T>
		inline bool read(T& out)
		{
			if(m_itr != nullptr && (m_itr + sizeof(T)) <= m_end)
			{
				std::memcpy(&out, m_itr, sizeof(T));
				m_itr = m_itr + sizeof(T);
				return true;
			}
			return false;
		}
		template <class T>
		inline bool peek(T& out) const
		{
			if(m_itr != nullptr && (m_itr + sizeof(T)) <= m_end)
			{
				std::memcpy(&out, m_itr, sizeof(T));
				return true;
			}
			return false;
		}
	public:
		const void* data() const
		{
			CLLIO_ASSERT(m_itr != nullptr);
			return m_itr;
		}

		inline std::size_t size() const
		{
			CLLIO_ASSERT(m_itr <= m_end);
			return (std::size_t)std::distance(m_itr, m_end);
		}

		inline void operator += (const std::size_t s)
		{
			m_itr += s;
			CLLIO_ASSERT(m_itr <= m_end);
		}
	protected:
		const byte_t* m_itr = nullptr;
		const byte_t* m_end = nullptr;
	};

}


