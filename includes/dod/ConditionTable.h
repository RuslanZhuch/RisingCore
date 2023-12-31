#pragma once

#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <cinttypes>
#include <span>

namespace Dod::CondTable
{

	enum class TriState : int8_t
	{
		SKIP = 0,
		FALSE, 
		TRUE,
	};

	struct Table
	{
		ImTable<uint32_t> xOrMasks;
		ImTable<uint32_t> ignoreMasks;
	};

	[[nodiscard]] static Table generate(const auto& tableSrc, const std::span<uint32_t> xOrMasksMem, const std::span<uint32_t> ignoreMasksMem) noexcept
	{

		Dod::ImTable<uint32_t> xOrMasks;
//		Dod::DataUtils::initFromArray(xOrMasks, xOrMasksMem);
		Dod::ImTable<uint32_t> ignoreMasks;
//		Dod::DataUtils::initFromArray(ignoreMasks, ignoreMasksMem);

		const auto tableSize{ static_cast<uint32_t>(tableSrc.size()) };

		for (uint32_t rowId{}; rowId < tableSize; ++rowId)
		{
			uint32_t xOr{};
			uint32_t ignore{};
			const auto rawSize{ static_cast<uint32_t>(tableSrc[static_cast<size_t>(rowId)].size()) };
			for (uint32_t colId{}; colId < rawSize; ++colId)
			{
				if (tableSrc[rowId][colId] == TriState::FALSE)
					xOr |= (1 << colId);
				if (tableSrc[rowId][colId] == TriState::SKIP)
					ignore |= (1 << colId);
			}
			for (uint32_t colId{ rawSize }; colId < 32; ++colId)
				ignore |= (1 << colId);

			Dod::DataUtils::populate(xOrMasks, xOr, true);
			Dod::DataUtils::populate(ignoreMasks, ignore, true);
		}

		return Table(xOrMasks, ignoreMasks);

	}

	template <typename T>
	void populateQuery(DTable<T>& query, const uint32_t inputs, const Table& table) noexcept
	{

		for (int32_t rowId{}; rowId < Dod::DataUtils::getNumFilledElements(table.xOrMasks); ++rowId)
		{
			const auto xOr{ Dod::DataUtils::get(table.xOrMasks, rowId) };
			const auto ignore{ Dod::DataUtils::get(table.ignoreMasks, rowId) };
			const uint32_t conditionMet{ (inputs ^ xOr) | ignore };
			const uint32_t czero{ conditionMet + 1 };
			const int32_t cmask{ static_cast<int32_t>(~(czero | static_cast<uint32_t>(-static_cast<int32_t>(czero)))) >> 31 };
			Dod::DataUtils::populate(query, rowId, static_cast<bool>(cmask));
		}

	}
	
	template <typename TInput, typename TOutput>
	void applyTransform(TOutput& target, const std::span<const TOutput> outputs, const Dod::ImTable<TInput>& query) noexcept
	{

		for (int32_t id{ 0 }; id < Dod::DataUtils::getNumFilledElements(query); ++id)
		{
			const auto outputId{ Dod::DataUtils::get(query, id) };
			target = outputs[static_cast<size_t>(outputId)];
		}

	}

};
