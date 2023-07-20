#pragma once

#include <dod/Buffers.h>
#include <dod/BufferUtils.h>
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
		const ImBuffer<uint32_t> xOrMasks;
		const ImBuffer<uint32_t> ignoreMasks;
	};

	[[nodiscard]] static Table generate(const auto& tableSrc, const std::span<uint32_t> xOrMasksMem, const std::span<uint32_t> ignoreMasksMem) noexcept
	{

		Dod::DBBuffer<uint32_t> xOrMasks;
		Dod::BufferUtils::initFromArray(xOrMasks, xOrMasksMem);
		Dod::DBBuffer<uint32_t> ignoreMasks;
		Dod::BufferUtils::initFromArray(ignoreMasks, ignoreMasksMem);

		for (int32_t rowId{}; rowId < tableSrc.size(); ++rowId)
		{
			uint32_t xOr{};
			uint32_t ignore{};
			for (int32_t colId{}; colId < tableSrc[rowId].size(); ++colId)
			{
				if (tableSrc[rowId][colId] == TriState::FALSE)
					xOr |= (1 << colId);
				if (tableSrc[rowId][colId] == TriState::SKIP)
					ignore |= (1 << colId);
			}
			for (size_t colId{ tableSrc[rowId].size() }; colId < 32; ++colId)
				ignore |= (1 << colId);

			Dod::BufferUtils::populate(xOrMasks, xOr, true);
			Dod::BufferUtils::populate(ignoreMasks, ignore, true);
		}

		return Table(Dod::BufferUtils::createImFromBuffer(xOrMasks), Dod::BufferUtils::createImFromBuffer(ignoreMasks));

	}

	template <typename T>
	void populateQuery(DBBuffer<T>& query, const uint32_t inputs, const Table& table) noexcept
	{

		for (int32_t rowId{}; rowId < Dod::BufferUtils::getNumFilledElements(table.xOrMasks); ++rowId)
		{
			const auto xOr{ Dod::BufferUtils::get(table.xOrMasks, rowId) };
			const auto ignore{ Dod::BufferUtils::get(table.ignoreMasks, rowId) };
			const uint32_t conditionMet{ (inputs ^ xOr) | ignore };
			const uint32_t czero{ conditionMet + 1 };
			const int32_t cmask{ static_cast<int32_t>(~(czero | static_cast<uint32_t>(-static_cast<int32_t>(czero)))) >> 31 };
			Dod::BufferUtils::populate(query, rowId, static_cast<bool>(cmask));
		}

	}
	
	template <typename TInput, typename TOutput>
	void applyTransform(TOutput& target, const std::span<const TOutput> outputs, const Dod::ImBuffer<TInput>& query) noexcept
	{

		for (int32_t id{ 0 }; id < Dod::BufferUtils::getNumFilledElements(query); ++id)
		{
			const auto outputId{ Dod::BufferUtils::get(query, id) };
			target = outputs[outputId];
		}

	}

};
