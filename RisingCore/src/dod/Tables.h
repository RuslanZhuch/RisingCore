#pragma once

#include "MemTypes.h"
#include "CommonData.h"

#include <tuple>

namespace Dod
{

	template <typename TFirst, typename ... TOther>
	struct DBTable : CommonData::Table
	{
		using types_t = typename std::tuple<TFirst, TOther...>;
		MemTypes::dataPoint_t dataBegin{ nullptr };
		//type_t* dataEnd{ nullptr };
		int32_t capacityEls{};
		int32_t numOfFilledEls{ 0 };
	};

}
