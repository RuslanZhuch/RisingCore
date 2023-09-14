#pragma once

#include "MemTypes.h"

#include <type_traits>

namespace Dod::CommonData
{

	struct Buffer {};

	template <typename T>
	concept CBuffer = requires(T)
	{
		typename T::type_t;
	};

	template <typename T>
	concept CCoreTable = requires(T)
	{
		typename T::types_t;
	};

	template <typename T>
	concept CTable = requires(T)
	{
		requires CCoreTable<T>;
		requires std::same_as<decltype(T::dataBegin), MemTypes::dataPoint_t>;
		requires std::same_as<decltype(T::capacityEls), int32_t>;
		requires std::same_as<decltype(T::numOfFilledEls), int32_t>;
	};

}
