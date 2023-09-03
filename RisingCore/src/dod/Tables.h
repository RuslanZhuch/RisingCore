#pragma once

#include "MemTypes.h"

#include <tuple>

namespace Dod
{

	template <typename TFirst, typename ... TOther>
	struct DBTable
	{
		using types_t = typename std::tuple<TFirst, TOther...>;
		MemTypes::dataPoint_t dataBegin{ nullptr };
		int32_t capacityEls{};
		int32_t numOfFilledEls{};
	};

}
