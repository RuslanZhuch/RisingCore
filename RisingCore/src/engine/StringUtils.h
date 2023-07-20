#pragma once

#include <string_view>

namespace Engine::StringUtils
{

	void assign(auto&& string, std::string_view inStr) noexcept
	{
		if (inStr.size() <= string.capacity)
			std::memcpy(string.data.data(), inStr.data(), inStr.size());
	}

}

[[nodiscard]] bool operator==(auto&& stringObject, const std::string_view right) noexcept
{
	return right.compare(stringObject.data.data()) == 0;
}
