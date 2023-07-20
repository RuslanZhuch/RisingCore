#pragma once

#include <array>
#include <string_view>

namespace Engine
{
	struct String
	{
		constexpr static int32_t capacity{ 64 };
		std::array<char, capacity> data{};
	};

}