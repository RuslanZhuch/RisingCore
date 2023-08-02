#pragma once

#include <string_view>

namespace Engine::StringUtils
{

	void assign(auto&& string, std::string_view inStr) noexcept
	{
		if (inStr.size() <= string.capacity)
			std::memcpy(string.internalData.data(), inStr.data(), inStr.size());
	}

}

template <typename BufferType>
[[nodiscard]] bool operator==(BufferType&& stringObject, const std::string_view right) noexcept requires requires (BufferType buff) {
	buff.internalData;
}
{
	return right.compare(stringObject.internalData.data()) == 0;
}
