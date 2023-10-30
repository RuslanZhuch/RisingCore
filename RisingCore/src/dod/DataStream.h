#pragma once

#include <dod/CommonData.h>
#include <dod/MemTypes.h>
#include <dod/DataUtils.h>

#include <concepts>

namespace Dod::DataStream
{

	template <Dod::CommonData::CMonoMutTable TTable, typename TData> requires std::is_trivial_v<TData>
	Dod::MemTypes::capacity_t serialize(TTable& dest, TData data, Dod::MemTypes::capacity_t header) noexcept
	{
		constexpr auto dataSize{ static_cast<decltype(header)>(sizeof(TData)) };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;

		const auto destCapacityInBytes{ static_cast<Dod::MemTypes::capacity_t>(Dod::DataUtils::getCapacity(dest)) * sizeof(destData_t) };
		const auto bHasSpace{ destCapacityInBytes - header >= dataSize };
		if (!bHasSpace)
			return header;

		std::memcpy(dest.dataBegin + header, &data, dataSize);

		return header + dataSize;
	}

	template <Dod::CommonData::CMonoMutTable TTable, typename TData> requires std::is_trivial_v<TData>
	Dod::MemTypes::capacity_t serialize(TTable& dest, const TData* data, int32_t numOfElements, Dod::MemTypes::capacity_t header) noexcept
	{

		const auto dataSize{ static_cast<decltype(header)>(sizeof(TData)) * numOfElements };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;

		constexpr auto bytesForCounter{ sizeof(numOfElements) };
		const auto destCapacityInBytes{ static_cast<Dod::MemTypes::capacity_t>(Dod::DataUtils::getCapacity(dest)) * sizeof(destData_t) };
		const auto bHasSpace{ destCapacityInBytes - header >= dataSize + bytesForCounter };
		if (!bHasSpace)
			return header;

		std::memcpy(dest.dataBegin + header, &numOfElements, bytesForCounter);
		std::memcpy(dest.dataBegin + header + bytesForCounter, data, dataSize);

		return header + bytesForCounter + dataSize;

	}

	template <typename TData, Dod::CommonData::CMonoMutTable TTable> requires std::is_trivial_v<TData>
	Dod::MemTypes::capacity_t deserialize(TTable& dest, Dod::MemTypes::capacity_t header, TData& data) noexcept
	{

		constexpr auto dataSize{ static_cast<decltype(header)>(sizeof(TData)) };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;
		const auto destCapacityInBytes{ static_cast<Dod::MemTypes::capacity_t>(Dod::DataUtils::getCapacity(dest) * sizeof(destData_t)) };
		if (header + dataSize > destCapacityInBytes)
			return header;

		std::memcpy(&data, dest.dataBegin + header, sizeof(TData));
		
		return header + dataSize;

	}

	template <typename TData, Dod::CommonData::CMonoMutTable TTable> requires std::is_trivial_v<TData>
	Dod::MemTypes::capacity_t deserialize(TTable& dest, Dod::MemTypes::capacity_t header, int32_t& numOfElements, TData*& data) noexcept
	{

		header = deserialize(dest, header, numOfElements);

		const auto dataSize{ static_cast<decltype(header)>(sizeof(TData)) * numOfElements };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;
		const auto destCapacityInBytes{ static_cast<Dod::MemTypes::capacity_t>(Dod::DataUtils::getCapacity(dest) * sizeof(destData_t)) };
		if (header + dataSize > destCapacityInBytes)
			return header;

		data = reinterpret_cast<TData*>(dest.dataBegin + header);

		return header + dataSize;

	}

}
