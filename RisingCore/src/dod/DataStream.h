#pragma once

#include <dod/CommonData.h>
#include <dod/MemTypes.h>
#include <dod/DataUtils.h>

#include <concepts>

namespace Dod::DataStream
{

	using header_t = int32_t;

	template <Dod::CommonData::CMonoMutTable TTable, typename TData> requires std::is_trivial_v<TData>
	header_t serialize(TTable& dest, TData data) noexcept
	{

		constexpr auto dataSize{ static_cast<header_t>(sizeof(TData)) };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;

		const auto destCapacityInBytes{ static_cast<header_t>(Dod::DataUtils::getCapacity(dest)) * sizeof(destData_t) };
		const auto bHasSpace{ destCapacityInBytes - dest.numOfFilledEls >= dataSize };
		
		if (bHasSpace)
		{
			std::memcpy(dest.dataBegin + dest.numOfFilledEls, &data, dataSize);
			dest.numOfFilledEls += dataSize;
		}
			
		return dest.numOfFilledEls;

	}

	template <Dod::CommonData::CMonoMutTable TTable, typename TData> requires std::is_trivial_v<TData>
	header_t serialize(TTable& dest, const TData* data, int32_t numOfElements) noexcept
	{

		const auto dataSize{ static_cast<header_t>(sizeof(TData)) * numOfElements };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;

		constexpr auto bytesForCounter{ sizeof(numOfElements) };
		const auto destCapacityInBytes{ static_cast<header_t>(Dod::DataUtils::getCapacity(dest)) * sizeof(destData_t) };
		const auto bHasSpace{ destCapacityInBytes - dest.numOfFilledEls >= dataSize + bytesForCounter };

		if (bHasSpace)
		{
			std::memcpy(dest.dataBegin + dest.numOfFilledEls, &numOfElements, bytesForCounter);
			std::memcpy(dest.dataBegin + dest.numOfFilledEls + bytesForCounter, data, dataSize);
			dest.numOfFilledEls += bytesForCounter + dataSize;
		}
		return dest.numOfFilledEls;

	}

	template <typename TData, Dod::CommonData::CMonoImTable TTable> requires std::is_trivial_v<TData>
	header_t deserialize(TTable src, header_t header, TData& data) noexcept
	{

		constexpr auto dataSize{ static_cast<decltype(header)>(sizeof(TData)) };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;
		const auto destCapacityInBytes{ static_cast<Dod::MemTypes::capacity_t>(Dod::DataUtils::getCapacity(src) * sizeof(destData_t)) };
		if (header + dataSize > destCapacityInBytes)
			return header;

		std::memcpy(&data, src.dataBegin + header, sizeof(TData));
		
		return header + dataSize;

	}

	template <typename TData, Dod::CommonData::CMonoImTable TTable> requires std::is_trivial_v<TData>
	header_t deserialize(TTable src, header_t header, int32_t& numOfElements, const TData*& data) noexcept
	{

		header = deserialize(Dod::ImTable(src), header, numOfElements);

		const auto dataSize{ static_cast<decltype(header)>(sizeof(TData)) * numOfElements };
		using destData_t = std::tuple_element_t<0, typename TTable::types_t>;
		const auto destCapacityInBytes{ static_cast<Dod::MemTypes::capacity_t>(Dod::DataUtils::getCapacity(src) * sizeof(destData_t)) };
		if (header + dataSize > destCapacityInBytes)
			return header;

		data = reinterpret_cast<const TData*>(src.dataBegin + header);

		return header + dataSize;

	}

}
