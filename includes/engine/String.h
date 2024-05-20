#pragma once

#pragma warning(push)

#pragma warning(disable : 4365)

#include <array>
#include <string_view>
#include <cstring>

#pragma warning(pop)

namespace Engine
{

	template <int32_t capacityArg>
	struct StringV
	{
		constexpr static int32_t capacity{ capacityArg };
		std::array<char, capacity> internalData;

		[[nodiscard]] bool operator==(const StringV<capacityArg>& other) const noexcept
		{
			return std::strcmp(this->internalData.data(), other.internalData.data()) == 0;
		}
	};

	using String = StringV<64>;

}