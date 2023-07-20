#pragma once

#include "dod/Buffers.h"

namespace Dod::Algorithms
{

	template <typename T>
	static void leftUniques(DBBuffer<T>& buffer) noexcept
	{

		if (Dod::BufferUtils::getNumFilledElements(buffer) == 0)
			return;

		T prevElValue{ buffer.dataBegin[1] };
		int32_t prevUniqueId{ 1 };
		for (int32_t elId{ 2 }; elId < Dod::BufferUtils::getNumFilledElements(buffer) + 1; ++elId)
		{
			const auto bIsUnique{ prevElValue != buffer.dataBegin[elId] };
			prevElValue = buffer.dataBegin[elId];
			if (bIsUnique)
			{
				const auto bGapExist{ elId - prevUniqueId >= 2 };
				if (bGapExist)
				{
					buffer.dataBegin[prevUniqueId + 1] = buffer.dataBegin[elId];
				}
				++prevUniqueId;
			}
		}

		buffer.numOfFilledEls = prevUniqueId;

	}

	template <typename T>
	static void getIntersections(DBBuffer<T>& resultBuffer, const auto& srcLeft, const auto& srcRight) noexcept
	{

		int32_t srcLeftId{ 0 };
		int32_t srcRightId{ 0 };

		while (srcLeftId < Dod::BufferUtils::getNumFilledElements(srcLeft) && srcRightId < srcRight.numOfFilledEls)
		{

			const auto leftValue{ BufferUtils::get(srcLeft, srcLeftId) };
			const auto rightValue{ BufferUtils::get(srcRight, srcRightId) };
			if (leftValue == rightValue)
			{
				BufferUtils::populate(resultBuffer, leftValue, true);
				++srcLeftId;
				++srcRightId;
			}
			else if (leftValue < rightValue)
				++srcLeftId;
			else 
				++srcRightId;
			
		}

	}

};
