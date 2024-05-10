#pragma once

#include "dod/CommonData.h"

#include <concepts>

namespace Dod::Algorithms
{

	template <typename T>
	concept CImBuffer = requires(T t)
	{
		{Dod::DataUtils::get(t, int32_t{})} -> std::convertible_to<typename T::type_t>;
	};

	static void leftUniques(Dod::CommonData::CMonoDTable auto& buffer) noexcept
	{

		if (Dod::DataUtils::getNumFilledElements(buffer) == 0)
			return;

		auto prevElValue{ Dod::DataUtils::get(buffer, 0) };
		int32_t prevUniqueId{ 0 };
		for (int32_t elId{ 1 }; elId < Dod::DataUtils::getNumFilledElements(buffer); ++elId)
		{
			const auto bIsUnique{ prevElValue != Dod::DataUtils::get(buffer, elId) };
			prevElValue = Dod::DataUtils::get(buffer, elId);
			if (bIsUnique)
			{
				const auto bGapExist{ elId - prevUniqueId >= 2 };
				if (bGapExist)
				{
					Dod::DataUtils::get(buffer, prevUniqueId + 1) = Dod::DataUtils::get(buffer, elId);
				}
				++prevUniqueId;
			}
		}

		buffer.numOfFilledEls = prevUniqueId + 1;

	}

	static void getSortedIndices(Dod::CommonData::CMonoDTable auto& sortedIndices, Dod::CommonData::CMonoImTable auto srcBuffer) noexcept requires
		std::is_same_v<std::tuple_element_t<0, typename std::decay_t<decltype(sortedIndices)>::types_t>, int32_t>
	{

		if (Dod::DataUtils::getNumFilledElements(srcBuffer) == 0)
			return;

		for (int32_t id{}; id < Dod::DataUtils::getNumFilledElements(srcBuffer); ++id)
			Dod::DataUtils::pushBack(sortedIndices, true, id);

		const auto dataBegin{ &Dod::DataUtils::get(sortedIndices, 0) };
		const auto endOffset{ Dod::DataUtils::getNumFilledElements(sortedIndices) };

		std::sort(dataBegin, dataBegin + endOffset, [&](int32_t leftId, int32_t rightId) -> bool {
			return Dod::DataUtils::get(srcBuffer, leftId) < Dod::DataUtils::get(srcBuffer, rightId);
		});

	}

	static void getSortedUniqueElsIndices(Dod::CommonData::CMonoDTable auto& sortedUniques, Dod::CommonData::CMonoImTable auto buffer) noexcept requires
		std::is_same_v<std::tuple_element_t<0, typename std::decay_t<decltype(sortedUniques)>::types_t>, int32_t>
	{

		if (Dod::DataUtils::getNumFilledElements(buffer) == 0)
			return;

		getSortedIndices(sortedUniques, buffer);
		const auto sortedBuffer{ Dod::DataUtils::createGuidedImTable(buffer, Dod::ImTable(sortedUniques)) };

		int32_t lastUniqueElId{ 0 };
		for (int32_t currentElId{ 0 }; currentElId < Dod::DataUtils::getNumFilledElements(sortedBuffer); )
		{
			int32_t repeats{};
			const auto lastElement{ Dod::DataUtils::get(sortedBuffer, currentElId) };
			const auto groupElementId{ Dod::DataUtils::get(sortedUniques, currentElId) };
			while (currentElId < Dod::DataUtils::getNumFilledElements(sortedBuffer))
			{
				const auto bEquals{ Dod::DataUtils::get(sortedBuffer, currentElId) == lastElement };
				repeats += bEquals;
				if (!bEquals)
					break;
				++currentElId;
			}
			Dod::DataUtils::get(sortedUniques, lastUniqueElId) = groupElementId;
			++lastUniqueElId;
		}

		sortedUniques.numOfFilledEls = lastUniqueElId;

	}

	template <typename T>
	[[nodiscard]] static int32_t getIndexByValue(Dod::CommonData::CMonoImTable auto src, T value, int32_t startIndex = 0) noexcept
	{

		const auto firstPassIndex{ startIndex };
		for (int32_t elId{ firstPassIndex }; elId < Dod::DataUtils::getNumFilledElements(src); ++elId)
		{
			if (Dod::DataUtils::get(src, elId) == value)
				return elId;
		}
		for (int32_t elId{ 0 }; elId < startIndex; ++elId)
		{
			if (Dod::DataUtils::get(src, elId) == value)
				return elId;
		}

		return -1;

	}

	template <typename T>
	static void getIndicesByValue(Dod::CommonData::CMonoDTable auto& indices, Dod::CommonData::CMonoImTable auto src, T value) noexcept requires
		std::is_same_v<std::tuple_element_t<0, typename std::decay_t<decltype(indices)>::types_t>, int32_t>
	{
		for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(src); ++elId)
		{
			const auto bFound{ Dod::DataUtils::get(src, elId) == value };
			Dod::DataUtils::pushBack(indices, bFound, elId);
		}
	}

	static void getIndicesByValues(Dod::CommonData::CMonoDTable auto& indices, Dod::CommonData::CMonoImTable auto src, Dod::CommonData::CMonoImTable auto values) noexcept requires
		std::is_same_v<std::tuple_element_t<0, typename std::decay_t<decltype(indices)>::types_t>, int32_t>
	{
		for (int32_t valueElId{}; valueElId < Dod::DataUtils::getNumFilledElements(values); ++valueElId)
		{
			const auto value{ Dod::DataUtils::get(values, valueElId) };
			for (int32_t srcElId{}; srcElId < Dod::DataUtils::getNumFilledElements(src); ++srcElId)
			{
				const auto bFound{ Dod::DataUtils::get(src, srcElId) == value };
				Dod::DataUtils::pushBack(indices, bFound, srcElId);
			}
		}
	}

	static void getIntersections(Dod::CommonData::CMonoDTable auto& resultBuffer, const auto& srcLeft, const auto& srcRight) noexcept
	{

		int32_t srcLeftId{ 0 };
		int32_t srcRightId{ 0 };

		while (srcLeftId < Dod::DataUtils::getNumFilledElements(srcLeft) && srcRightId < srcRight.numOfFilledEls)
		{

			const auto leftValue{ DataUtils::get(srcLeft, srcLeftId) };
			const auto rightValue{ DataUtils::get(srcRight, srcRightId) };
			if (leftValue == rightValue)
			{
				DataUtils::pushBack(resultBuffer, true, leftValue);
				++srcLeftId;
				++srcRightId;
			}
			else if (leftValue < rightValue)
				++srcLeftId;
			else 
				++srcRightId;
			
		}

	}

	static void countUniques(Dod::CommonData::CMonoDTable auto& counters, Dod::CommonData::CMonoImTable auto srcSortedBuffer) noexcept requires
		std::is_same_v<std::tuple_element_t<0, typename std::decay_t<decltype(counters)>::types_t>, int32_t>
	{

		if (Dod::DataUtils::getNumFilledElements(srcSortedBuffer) == 0)
			return;

		using table_t = decltype(srcSortedBuffer);
		using type_t = std::tuple_element_t<0, typename table_t::types_t>;

		type_t prevValue{ Dod::DataUtils::get(srcSortedBuffer, 0) };
		int32_t counter{ 1 };
		for (int32_t id{ 1 }; id < Dod::DataUtils::getNumFilledElements(srcSortedBuffer); ++id)
		{
			const auto currValue{ Dod::DataUtils::get(srcSortedBuffer, id) };
			const auto bDiffers{ currValue != prevValue };
			prevValue = currValue;
			counter += !bDiffers;

			Dod::DataUtils::pushBack(counters, bDiffers, counter);
			counter -= (counter - 1) * bDiffers;
		}

		Dod::DataUtils::pushBack(counters, counter > 0, counter);

	}

};
