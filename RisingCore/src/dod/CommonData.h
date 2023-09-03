#pragma once

#include "MemTypes.h"

#include <type_traits>

namespace Dod::CommonData
{

	struct Buffer {};

	template <typename T>
	concept CBuffer = std::is_base_of_v<Buffer, T>;

	template <typename T>
	concept CCoreTable = requires(T)
	{
		typename T::types_t;
	};

	template <typename T>
	concept CTable = requires(T)
	{
		requires CCoreTable<T>;
		std::same_as<decltype(T::dataBegin), MemTypes::dataPoint_t>;
		std::same_as<decltype(T::capacityEls), int32_t>;
		std::same_as<decltype(T::numOfFilledEls), int32_t>;
	};

}
