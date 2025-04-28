#pragma once

#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>
#include <dod/DataUtils.h>

#include <array>

namespace Engine::LocalTable
{

	struct MemoryPool
	{
		MemoryPool(size_t numOfBytes)
		{
			memory.resize(numOfBytes);
			dataBegin = memory.data();
			dataEnd = memory.data() + numOfBytes;
		}

		Dod::MemTypes::dataPoint_t dataBegin{ };
		Dod::MemTypes::dataPoint_t dataEnd{ };
		std::vector<Dod::MemTypes::data_t> memory;
	};

	template <size_t numOfBytes>
	struct StackPool
	{
		StackPool()
		{
			dataBegin = memory.data();
			dataEnd = memory.data() + numOfBytes;
		}

		Dod::MemTypes::dataPoint_t dataBegin{ };
		Dod::MemTypes::dataPoint_t dataEnd{ };
		std::array<Dod::MemTypes::data_t, numOfBytes> memory;
	};

	template <typename T>
	concept CMemoryPool = requires(T pool) {
		{ pool.dataBegin } -> std::convertible_to<Dod::MemTypes::dataPoint_t>;
		{ pool.dataEnd } -> std::convertible_to<Dod::MemTypes::dataPoint_t>;
	};

	template <typename TFirst, typename ... TOther>
	[[nodiscard]] auto createAndUpdateHeader(
		Dod::MemTypes::capacity_t numOfElements,
		CMemoryPool auto& pool,
		Dod::MemTypes::capacity_t& memoryHeader
	)
	{
		using types_t = std::tuple<TFirst, TOther...>;
		const auto capacityBytes{ Dod::DataUtils::computeCapacityInBytes<types_t>(numOfElements) };

		Dod::DTable<TFirst, TOther...> table;
		Dod::DataUtils::initFromMemory(table, numOfElements, Dod::MemUtils::stackAquire(pool, capacityBytes, 64, memoryHeader));

		return table;
	}

	template <typename TFirst, typename ... TOther>
	[[nodiscard]] auto create(
		Dod::MemTypes::capacity_t numOfElements,
		CMemoryPool auto& pool
	)
	{
		Dod::MemTypes::capacity_t memoryHeader{};
		return createAndUpdateHeader<TFirst, TOther...>(numOfElements, pool, memoryHeader);
	}

	template <typename TFirst, typename ... TOther>
	[[nodiscard]] auto create(
		Dod::MemTypes::capacity_t numOfElements,
		CMemoryPool auto& pool,
		Dod::MemTypes::capacity_t memoryHeader
	)
	{
		return createAndUpdateHeader<TFirst, TOther...>(numOfElements, pool, memoryHeader);
	}

};
