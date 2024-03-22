#pragma once

#include <cinttypes>

namespace Engine
{

	template <typename Type, uint32_t capacity>
	struct StaticVector
	{
		using type_t = Type;
		constexpr static uint32_t cap = capacity;
		uint32_t size;
		Type data[capacity];
	};

	template <typename Type, int32_t capacity>
	struct StaticVectorSmall
	{
		using type_t = Type;
		constexpr static uint8_t cap = capacity;
		uint8_t size;
		Type data[capacity];
		static_assert(capacity > 0 && capacity == static_cast<decltype(capacity)>(cap));
	};

}
