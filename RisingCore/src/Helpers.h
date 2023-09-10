#pragma once

namespace RisingCore::Helpers
{

	template <size_t N, size_t Current = 0>
	constexpr void constexprLoop(auto func) {
		if constexpr (Current < N) {
			func.operator()<Current>();
			constexprLoop<N, Current + 1>(func);
		}
	}

	template <typename TypesList, size_t largetsAlignment = 0, size_t Index = 0>
	[[nodiscard]] consteval auto findLargestAlignment()
	{
		constexpr auto numOfTypes{ std::tuple_size_v<TypesList> };
		if constexpr (Index < numOfTypes)
		{
			using currentType_t = std::tuple_element_t<Index, TypesList>;
			constexpr auto currentTypeAlignment{ alignof(currentType_t) };
			constexpr auto newLargestAlignment{ std::max(largetsAlignment, currentTypeAlignment) };
			return findLargestAlignment<TypesList, newLargestAlignment, Index + 1>();
		}
		else
		{
			return largetsAlignment;
		}
	}

	template <typename ...Types>
	[[nodiscard]] consteval auto findLargestAlignmentFlat()
	{
		using types_t = std::tuple<Types...>;
		return findLargestAlignment<types_t>();
	}

}
