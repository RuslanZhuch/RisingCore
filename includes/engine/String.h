#pragma once

#pragma warning(push)

#pragma warning(disable : 4365)

#include <array>
#include <string_view>
#include <cstring>

#pragma warning(pop)

namespace Engine
{
	struct String
	{
		constexpr static int32_t capacity{ 64 };
		std::array<char, capacity> internalData;

		[[nodiscard]] bool operator==(const String& other) const noexcept
		{
			return std::strcmp(this->internalData.data(), other.internalData.data()) == 0;
		}

	};

}