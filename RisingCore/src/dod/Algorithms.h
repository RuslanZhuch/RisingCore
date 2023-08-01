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

	template <typename T>
	static void getSortedIndices(DBBuffer<int32_t>& sortedIndices, ImBuffer<T> srcBuffer) noexcept
	{

		if (Dod::BufferUtils::getNumFilledElements(srcBuffer) == 0)
			return;

		for (int32_t id{}; id < Dod::BufferUtils::getNumFilledElements(srcBuffer); ++id)
			Dod::BufferUtils::populate(sortedIndices, id, true);

		const auto beginOffset{ 1 };
		const auto endOffset{ 1 + Dod::BufferUtils::getNumFilledElements(sortedIndices) };

		std::sort(sortedIndices.dataBegin + beginOffset, sortedIndices.dataBegin + endOffset, [&](int32_t leftId, int32_t rightId) -> bool {
			return Dod::BufferUtils::get(srcBuffer, leftId) < Dod::BufferUtils::get(srcBuffer, rightId);
		});

	}

	template <typename BufferType>
	static void countUniques(DBBuffer<int32_t>& counters, BufferType srcSortedBuffer) noexcept requires requires(BufferType) {
		std::is_const_v<decltype(BufferType::dataBegin)>;
		std::is_const_v<decltype(BufferType::dataEnd)>;
	}
	{

		if (Dod::BufferUtils::getNumFilledElements(srcSortedBuffer) == 0)
			return;

		using type_t = BufferType::type_t;

		type_t prevValue{ Dod::BufferUtils::get(srcSortedBuffer, 0) };
		int32_t counter{ 1 };
		for (int32_t id{ 1 }; id < Dod::BufferUtils::getNumFilledElements(srcSortedBuffer); ++id)
		{
			const auto currValue{ Dod::BufferUtils::get(srcSortedBuffer, id) };
			const auto bDiffers{ currValue != prevValue };
			prevValue = currValue;
			counter += !bDiffers;

			Dod::BufferUtils::populate(counters, counter, bDiffers);
			counter -= (counter - 1) * bDiffers;
		}

		Dod::BufferUtils::populate(counters, counter, counter > 0);

	}

};
