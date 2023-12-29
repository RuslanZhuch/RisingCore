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
	concept CTable = requires(T)
	{
		typename T::types_t;
		{std::same_as<decltype(T::capacityEls), const int32_t>};
	};

	template <typename Left, typename Right>
	concept CTablesStructureIsSame = requires(Left, Right)
	{
		{std::same_as<typename std::decay_t<Left>::types_t, typename std::decay_t<Right>::types_t>};
	};

	template <typename T>
	concept CImTable = requires(T)
	{
		requires CTable<T>;
		requires std::same_as<decltype(T::dataBegin), MemTypes::dataConstPoint_t>;
		requires std::same_as<decltype(T::numOfElements), int32_t>;
	};

	template <typename T>
	concept CMutTable = requires(T)
	{
		requires CTable<T>;
		requires std::same_as<decltype(T::dataBegin), MemTypes::dataPoint_t>;
		requires std::same_as<decltype(T::numOfElements), int32_t>;
	};

	template <typename T>
	concept CDTable = requires(T)
	{
		requires CTable<T>;
		requires std::same_as<decltype(T::dataBegin), MemTypes::dataPoint_t>;
		requires std::same_as<decltype(T::numOfFilledEls), int32_t>;
	};

	template <typename T>
	concept CMonoData = std::tuple_size_v<typename T::types_t> == 1;

	template <typename T>
	concept CMonoTable = CTable<T> && CMonoData<T>;

	template <typename T>
	concept CMonoDTable = CDTable<T> && CMonoData<T>;

	template <typename T>
	concept CMonoMutTable = CMutTable<T> && CMonoData<T>;

	template <typename T>
	concept CMonoImTable = CImTable<T> && CMonoData<T>;

	template <typename T>
	concept CDataGuided = requires(T)
	{
		requires CMonoImTable<decltype(T::guid)>;
		requires std::same_as<std::tuple_element_t<0, typename decltype(T::guid)::types_t>, int32_t>;
	};

	template <typename Tuple, bool isTrivial = true, size_t id = 0>
	[[nodiscard]] consteval auto getAllTypesAreTrivial()
	{
		constexpr auto totalElements{ std::tuple_size_v<Tuple> };
		if constexpr (id < totalElements)
		{
			using types_t = Tuple;
			using type_t = std::tuple_element_t<id, types_t>;
			const auto newIsTrivial{ isTrivial && std::is_trivial_v<type_t> };
			return getAllTypesAreTrivial<Tuple, newIsTrivial, id + 1>();
		}
		else
			return isTrivial;
	}

	template <typename T>
	concept CTrivialTable = getAllTypesAreTrivial<typename std::decay_t<T>::types_t>();

}
