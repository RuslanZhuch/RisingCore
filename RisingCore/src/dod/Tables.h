#pragma once

#include "MemTypes.h"

#include "CommonData.h"
#include <tuple>

namespace Dod
{

	template <typename TFirst, typename ... TOther>
	struct ImTable;
	template <typename TFirst, typename ... TOther>
	struct MutTable;

	template <typename TFirst, typename ... TOther>
	struct DTable
	{
		DTable() = default;
		DTable(const DTable&) = delete;
		DTable(DTable&&) noexcept = default;

		DTable& operator=(const DTable&) = delete;
		DTable& operator=(DTable&&) = default;

		using types_t = typename std::tuple<TFirst, TOther...>;
		using innerTables_t = typename std::tuple<MutTable<TFirst>, MutTable<TOther>...>;
		MemTypes::dataPoint_t dataBegin{ nullptr };
		int32_t capacityEls{};
		int32_t numOfFilledEls{};
	};

	static_assert(std::is_default_constructible_v<DTable<int>>);
	static_assert(!std::is_copy_constructible_v<DTable<int>>);
	static_assert(!std::is_copy_assignable_v<DTable<int>>);
	static_assert(std::is_move_constructible_v<DTable<int>>);
	static_assert(std::is_move_assignable_v<DTable<int>>);

	template <typename TFirst, typename ... TOther>
	struct MutTable
	{
		MutTable() = default;
		explicit MutTable(const DTable<TFirst, TOther...>& other) noexcept
			:
			dataBegin(other.dataBegin),
			capacityEls(other.numOfFilledEls)
		{}
		using types_t = typename std::tuple<TFirst, TOther...>;
		using innerTables_t = typename std::tuple<MutTable<TFirst>, MutTable<TOther>...>;
		MemTypes::dataPoint_t dataBegin{ nullptr };
		int32_t capacityEls{};
	};

	template <typename TFirst, typename ... TOther>
	struct ImTable
	{
		ImTable() = default;
		explicit ImTable(MutTable<TFirst, TOther...> other) noexcept
			:
			dataBegin(other.dataBegin),
			capacityEls(other.capacityEls)
		{}
		explicit ImTable(const DTable<TFirst, TOther...>& other) noexcept
			:
			dataBegin(other.dataBegin),
			capacityEls(other.numOfFilledEls)
		{}
		using types_t = typename std::tuple<TFirst, TOther...>;
		using innerTables_t = typename std::tuple<ImTable<TFirst>, ImTable<TOther>...>;
		MemTypes::dataConstPoint_t dataBegin{ nullptr };
		int32_t capacityEls{};
		int32_t pad{};
	};

	template <typename TFirst, typename ... TOther>
	struct ImTableGuided
	{
		ImTableGuided() = default;
		explicit ImTableGuided(MutTable<TFirst, TOther...> other) noexcept
			:
			dataBegin(other.dataBegin),
			capacityEls(other.capacityEls),
			guid(other.guid)
		{}
		explicit ImTableGuided(const DTable<TFirst, TOther...>& other) noexcept
			:
			dataBegin(other.dataBegin),
			capacityEls(other.numOfFilledEls),
			guid(other.guid)
		{}
		using types_t = typename std::tuple<TFirst, TOther...>;
		using innerTables_t = typename std::tuple<ImTable<TFirst>, ImTable<TOther>...>;
		MemTypes::dataConstPoint_t dataBegin{ nullptr };
		ImTable<int32_t> guid;
		int32_t capacityEls{};
	};



//	template <typename TFirst, typename ... TOther>
//	ImTable(MutTable<TFirst, TOther...>) -> ImTable<TFirst, TOther...>;
//	template <typename TFirst, typename ... TOther>
//	ImTable(DTable<TFirst, TOther...>) -> ImTable<TFirst, TOther...>;

}
