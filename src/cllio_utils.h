
#pragma once

#include "cllio_config.h"

#define CLLIO_UNUSED(V) ((const void*)&V)

namespace cllio
{
	//--------------------------------------------------------------------------------------------------------

	template <class F, class S>
	union UnionCast
	{
		F 	first;
		S 	second;
	};

	//--------------------------------------------------------------------------------------------------------

	struct detail
	{
	#if CLLIO_CAST_FUNCTION == 2
		
		template <std::size_t N, class O>
		union buffer
		{
			uint8_t data[N];
			O 		value;
			constexpr std::size_t size() const { return N; }
		};
		inline static void uint16_to_binary(buffer<2,uint16_t> & storage, const uint16_t value)
		{
			storage.value = value;
		}
		inline static void uint32_to_binary(buffer<4,uint32_t> & storage, const uint32_t value)
		{
			storage.value = value;
		}
		inline static void uint64_to_binary(buffer<8,uint64_t> & storage, const uint64_t value)
		{
			storage.value = value;
		}

		inline static uint16_t uint16_from_binary(const buffer<2,uint16_t> & storage)
		{
			return storage.value;
		}
		inline static uint32_t uint32_from_binary(const buffer<4,uint32_t> & storage)
		{
			return storage.value;
		}
		inline static uint64_t uint64_from_binary(const buffer<8,uint64_t> & storage)
		{
			return storage.value;
		}
	#elif CLLIO_CAST_FUNCTION == 1
		template <std::size_t N, class O>
		struct buffer
		{
			uint8_t data[N];
			constexpr std::size_t size() const { return N; }
		};
		inline static void uint16_to_binary(buffer<2,uint16_t> & storage, const uint16_t value)
		{
			std::memcpy(&storage.data[0],&value,sizeof(value));
		}
		inline static void uint32_to_binary(buffer<4,uint32_t> & storage, const uint32_t value)
		{
			std::memcpy(&storage.data[0],&value,sizeof(value));
		}
		inline static void uint64_to_binary(buffer<8,uint64_t> & storage, const uint64_t value)
		{
			std::memcpy(&storage.data[0],&value,sizeof(value));
		}

		inline static uint16_t uint16_from_binary(const buffer<2,uint16_t> & storage)
		{
			uint16_t value;
			std::memcpy(&value,&storage.data[0],sizeof(value));
			return value;
		}
		inline static uint32_t uint32_from_binary(const buffer<4,uint32_t> & storage)
		{
			uint32_t value;
			std::memcpy(&value,&storage.data[0],sizeof(value));
			return value;
		}
		inline static uint64_t uint64_from_binary(const buffer<8,uint64_t> & storage)
		{
			uint64_t value;
			std::memcpy(&value,&storage.data[0],sizeof(value));
			return value;
		}
	#else
		template <std::size_t N, class O>
		struct buffer
		{
			uint8_t data[N];
			constexpr std::size_t size() const { return N; }
		};
		static void uint16_to_binary(buffer<2,uint16_t> & storage, const uint16_t i);
		static void uint32_to_binary(buffer<4,uint32_t> & storage, const uint32_t i);
		static void uint64_to_binary(buffer<8,uint64_t> & storage, const uint64_t i);

		static uint16_t uint16_from_binary(const buffer<2,uint16_t> & storage);
		static uint32_t uint32_from_binary(const buffer<4,uint32_t> & storage);
		static uint64_t uint64_from_binary(const buffer<8,uint64_t> & storage);
	#endif

		inline static uint16_t int16_from_binary(const buffer<2, uint16_t>& storage)
		{
			UnionCast<uint16_t,uint16_t> data;
			data.first = uint16_from_binary(storage);
			return data.second;
		}
		inline static uint32_t int32_from_binary(const buffer<4, uint32_t>& storage)
		{
			UnionCast<uint32_t,uint32_t> data;
			data.first = uint32_from_binary(storage);
			return data.second;
		}
		inline static uint64_t int64_from_binary(const buffer<8, uint64_t>& storage)
		{
			UnionCast<uint64_t,uint64_t> data;
			data.first = uint64_from_binary(storage);
			return data.second;
		}
		 
	};

}