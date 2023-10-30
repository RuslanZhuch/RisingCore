#pragma once

#pragma warning(push)

#pragma warning(disable : 4365)

#include <array>
#include <string_view>

#pragma warning(pop)

namespace Engine
{
	struct String
	{
		constexpr static int32_t capacity{ 64 };
		std::array<char, capacity> internalData;
	};

}