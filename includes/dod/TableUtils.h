#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include "CommonData.h"
#include "Tables.h"

#include "Helpers.h"

#include <type_traits>
#include <concepts>
#include <cmath>

namespace Dod::DataUtils
{

	template <typename Tuple, size_t Index = 0>
	constexpr size_t computeTypesSize() {
		if constexpr (Index < std::tuple_size_v<Tuple>)
		{
			using type_t = std::tuple_element_t<Index, Tuple>;
			return sizeof(type_t) + computeTypesSize<Tuple, Index + 1>();
		}
		else
			return 0;
	}


//	template <typename Types>
//	[[nodiscard]] consteval auto computeDeadbucketSizeInBytes() noexcept
//	{
//		return RisingCore::Helpers::findLargestTypeSize<Types>() + RisingCore::Helpers::findLargestAlignment<Types>();
//	}

	template <typename Type>
	[[nodiscard]] constexpr auto computeTypeOffset(const Type* ptr) noexcept
	{
		constexpr auto alignment{ alignof(Type) };
		const auto address{ reinterpret_cast<std::uintptr_t>(ptr) };
		return address % alignment;
	}

	template <typename Types>
	[[nodiscard]] MemTypes::capacity_t computeCapacityInBytes(int32_t numOfElements) noexcept
	{

		if (numOfElements == 0)
			return MemTypes::capacity_t{};

		using types_t = Types;
//		constexpr auto deadBucketSize{ computeDeadbucketSizeInBytes<types_t>() };

		MemTypes::capacity_t needBytes{ 0 };
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t columnId>() {
			using type_t = std::tuple_element_t<columnId, types_t>;
			
			const auto rawBytesForColumn{ static_cast<MemTypes::capacity_t>(sizeof(type_t) * (numOfElements)) };
			const auto numOfCellsForColumn{ static_cast<MemTypes::capacity_t>(std::ceilf(rawBytesForColumn / 64.f)) };
			const auto pureBytesForColumn{ numOfCellsForColumn * 64 };
			needBytes += pureBytesForColumn;
		});

		return needBytes;

	}

	[[nodiscard]] static auto roundToCells(MemTypes::capacity_t bytes) noexcept
	{
		const auto bytesF{ static_cast<float>(bytes) };
		const auto numOfCellsForColumn{ static_cast<MemTypes::capacity_t>(std::ceil(bytesF / 64.f)) };
		return numOfCellsForColumn * 64;
	}

	[[nodiscard]] auto getNumFilledElements(const CommonData::CTable auto& table) noexcept
	{
		if constexpr (requires { table.numOfFilledEls; })
			return table.numOfFilledEls;
		else
			return static_cast<int32_t>(table.numOfElements);
	}

	void initTableFromMemoryImpl(CommonData::CTable auto& dbTable, int32_t capacityEls, int32_t numOfElements, auto&& actualData) noexcept
	{

		using tableType_t = typename std::decay_t<decltype(dbTable)>;
		using types_t = tableType_t::types_t;

		const auto needBytes{ computeCapacityInBytes<types_t>(capacityEls) };
		const auto alignmentOffset{ MemUtils::getAlignOffset(actualData.dataBegin, 64) };

		dbTable.dataBegin = actualData.dataBegin + alignmentOffset;

		const auto capacityInBytes{ static_cast<MemTypes::capacity_t>(actualData.dataEnd - actualData.dataBegin) };
		dbTable.capacityEls = capacityEls * (needBytes + alignmentOffset <= capacityInBytes);

		if constexpr (requires { dbTable.numOfFilledEls; })
			dbTable.numOfFilledEls = 0;
		else if constexpr (requires { dbTable.numOfElements; })
			dbTable.numOfElements = numOfElements;

	}


	void initFromMemory(CommonData::CDTable auto& dbTable, int32_t numOfElements, const auto& memSpan) noexcept
	{
		initTableFromMemoryImpl(dbTable, numOfElements, 0, memSpan);
	}

	void initFromMemory(CommonData::CMutTable auto& dbTable, int32_t capacityEls, int32_t numOfElements, const auto& memSpan) noexcept
	{
		initTableFromMemoryImpl(dbTable, capacityEls, numOfElements, memSpan);
	}

	void initFromMemory(CommonData::CImTable auto& dbTable, int32_t capacityEls, int32_t numOfElements, const auto& memSpan) noexcept
	{
		initTableFromMemoryImpl(dbTable, capacityEls, numOfElements, memSpan);
	}

	template<int32_t collumnId>
	[[nodiscard]] auto get(const CommonData::CTable auto& table) noexcept
	{

		using tableType_t = std::decay_t<decltype(table)>;
		using types_t = tableType_t::types_t;

		decltype(table.dataBegin) rowMemPosition{ table.dataBegin + MemUtils::getAlignOffset(table.dataBegin, 64) };
		RisingCore::Helpers::constexprLoop<collumnId>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };

			const auto pureBytesForColumn{ roundToCells(table.capacityEls * columnTypeSize) };

			rowMemPosition += pureBytesForColumn;
		});

		using returnType_t = std::tuple_element_t<static_cast<size_t>(collumnId), types_t>;
		rowMemPosition += MemUtils::getAlignOffset(rowMemPosition, alignof(returnType_t));

		constexpr auto returnTypeSize{ sizeof(returnType_t) };
		const auto pureBytesForColumn{ roundToCells(table.capacityEls * returnTypeSize) };
		struct Span
		{
			decltype(rowMemPosition) dataBegin{ rowMemPosition };
			decltype(rowMemPosition) dataEnd{ rowMemPosition + pureBytesForColumn };
		};

		if constexpr (Dod::CommonData::CDTable<tableType_t>)
		{
			Dod::MutTable<returnType_t> out;
			initFromMemory(out, table.capacityEls, Dod::DataUtils::getNumFilledElements(table), Span{});
			return out;
		}
		else if constexpr (Dod::CommonData::CImTable<std::decay_t<decltype(table)>>)
		{
			Dod::ImTable<returnType_t> out;
			initFromMemory(out, table.capacityEls, Dod::DataUtils::getNumFilledElements(table), Span{});
			return out;
		}
		else
		{
			Dod::MutTable<returnType_t> out;
			initFromMemory(out, table.capacityEls, Dod::DataUtils::getNumFilledElements(table), Span{});
			return out;
		}

	}

	template<typename TReturn>
	[[nodiscard]] auto getImpl(const CommonData::CTable auto& table, MemTypes::capacity_t beginElIndex, MemTypes::capacityEls_t numOfElements) noexcept
	{

		TReturn buffersPack;

		using dataPointType_t = decltype(table.dataBegin);
		dataPointType_t rowMemPosition{ table.dataBegin + MemUtils::getAlignOffset(table.dataBegin, 64) };
		RisingCore::Helpers::constexprLoop<std::tuple_size_v<TReturn>>([&]<size_t columnId>() {
			auto& bufferPack{ std::get<columnId>(buffersPack) };
			using pack_t = std::decay_t<decltype(bufferPack)>;
			using packType_t = pack_t::types_t;

			constexpr auto columnTypeSize{ sizeof(packType_t) };
			struct Span
			{
				dataPointType_t dataBegin{ nullptr };
				dataPointType_t dataEnd{ nullptr };
			};

			const auto spanBeginOffset{ beginElIndex * columnTypeSize };

			const auto pureBytesForColumn{ roundToCells(table.capacityEls * columnTypeSize) };
			initFromMemory(bufferPack, table.capacityEls, numOfElements, Span(rowMemPosition + spanBeginOffset, rowMemPosition + spanBeginOffset + pureBytesForColumn));
			rowMemPosition += pureBytesForColumn;

		});

		return buffersPack;

	}

	template <typename TReturn>
	[[nodiscard]] auto get(const CommonData::CTable auto& table, MemTypes::capacity_t beginElIndex, MemTypes::capacityEls_t numOfElements) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;
		if constexpr (Dod::CommonData::CImTable<decltype(table)>)
			return getImpl<TReturn>(table, beginElIndex, numOfElements);
		else
			return getImpl<TReturn>(table, beginElIndex, numOfElements);

	}

	template <CommonData::CTable Table>
	[[nodiscard]] auto get(const Table& table) noexcept -> Table::innerTables_t
	{
		if constexpr (CommonData::CImTable<Table>)
			return get<typename Table::innerTables_t>(table, 0, getNumFilledElements(table));
		else if constexpr (CommonData::CDTable<Table>)
			return get<typename Table::innerTables_t>(table, 0, getNumFilledElements(table));
		else if constexpr (CommonData::CMutTable<Table>)
			return get<typename Table::innerTables_t>(table, 0, getNumFilledElements(table));
	}

//	template <CommonData::CDTable Table>
//	[[nodiscard]] auto get(const Table& table) noexcept -> Table::innerTables_t
//	{
//		return get<typename Table::innerTables_t>(table, 0, table.numOfFilledEls);
//	}
//
//	template <CommonData::CMutTable Table>
//	[[nodiscard]] auto get(const Table& table) noexcept -> Table::innerTables_t
//	{
//		return get<typename Table::innerTables_t>(table, 0, table.capacityEls);
//	}
//
//	template <CommonData::CImTable Table>
//	[[nodiscard]] auto get(const Table& table) noexcept -> Table::innerTables_t
//	{
//		return get<typename Table::innerTables_t>(table, 0, table.capacityEls);
//	}

	template <CommonData::CMonoTable TableType>
	[[nodiscard]] auto& get(const TableType& table, int32_t elId) noexcept
	{

		if constexpr (Dod::CommonData::CDataGuided<TableType>)
			elId = Dod::DataUtils::get(table.guid, elId);

		using type_t = std::tuple_element_t<0, typename TableType::types_t>;
		if constexpr (CommonData::CMonoImTable<std::decay_t<decltype(table)>>)
		{
			return *(reinterpret_cast<const type_t*>(table.dataBegin) + elId);
		}
		else
		{
			return *(reinterpret_cast<type_t*>(table.dataBegin) + elId);
		}

	}

	void pushBack(CommonData::CDTable auto& table, bool strobe, auto&& ... value) noexcept requires
		CommonData::CTrivialTable<decltype(table)>
	{

		if (Dod::DataUtils::getNumFilledElements(table) >= table.capacityEls)
			return;

		const auto elementPosition{ table.numOfFilledEls };
		table.numOfFilledEls += int32_t(1) * strobe;

		using tableType_t = typename std::decay_t<decltype(table)>;
		using types_t = tableType_t::types_t;

		auto memoryOffset{ MemUtils::getAlignOffset(table.dataBegin, 64) };

		MemTypes::dataPoint_t rowMemPosition{ table.dataBegin };
		(std::invoke([&] {
			using valueType_t = std::decay_t<decltype(value)>;
			const auto inColumnMemoryOffset{ memoryOffset + elementPosition * sizeof(valueType_t) };
			const auto memoryPosition{ reinterpret_cast<valueType_t*>(table.dataBegin + inColumnMemoryOffset) };

			*memoryPosition = std::move(value);

			const auto pureBytesForColumn{ roundToCells((table.capacityEls) * sizeof(valueType_t)) };
			memoryOffset += pureBytesForColumn;
		}), ...);

	}

	void populate(CommonData::CDTable auto& table, int32_t numOfElements, std::invocable<int32_t> auto&& ... filler) noexcept requires
		CommonData::CTrivialTable<decltype(table)>
	{


		if (table.numOfFilledEls + numOfElements > table.capacityEls)
			return;

		using tableType_t = typename std::decay_t<decltype(table)>;
		using types_t = tableType_t::types_t;
		static_assert(sizeof...(filler) == std::tuple_size_v<types_t>, "Filler args don't match table types");
		const auto elementPosition{ table.numOfFilledEls };
		auto memoryOffset{ MemUtils::getAlignOffset(table.dataBegin, 64) };
		(std::invoke([&] {
			using valueType_t = std::invoke_result_t<decltype(filler), int32_t>;
			for (int32_t elId{}; elId < numOfElements; ++elId)
			{
				const auto inColumnMemoryOffset{ memoryOffset + (elementPosition + elId) * sizeof(valueType_t) };
				const auto memoryPosition{ reinterpret_cast<valueType_t*>(table.dataBegin + inColumnMemoryOffset) };
				*memoryPosition = std::invoke(filler, elId);
			}
			const auto pureBytesForColumn{ roundToCells((table.capacityEls) * sizeof(valueType_t)) };
			memoryOffset += pureBytesForColumn;
		}), ...);
		table.numOfFilledEls += numOfElements;
	}

//	auto construct(CommonData::CDTable auto& table, int32_t numOfElements) noexcept requires 
//		CommonData::CTrivialTable<decltype(table)>
//	{
//
//		if (table.numOfFilledEls + numOfElements > table.capacityEls)
//			return get(table, 0, 0);
//
//		using tableType_t = typename std::decay_t<decltype(table)>;
//		using types_t = tableType_t::types_t;
//		const auto elementPosition{ table.numOfFilledEls };
//		size_t memoryOffset{ MemUtils::getAlignOffset(table.dataBegin, 64) };
//		RisingCore::Helpers::constexprLoop<std::tuple_size_v<types_t>>([&]<size_t currColId>() {
//			using columnType_t = std::tuple_element_t<currColId, types_t>;
//
//			const auto inColumnMemoryOffset{ memoryOffset + (elementPosition) * sizeof(columnType_t) };
//			const auto memoryPosition{ reinterpret_cast<columnType_t*>(table.dataBegin + inColumnMemoryOffset) };
//			std::memset(memoryPosition, 0, numOfElements * sizeof(columnType_t));
//
//			const auto pureBytesForColumn{ roundToCells((table.capacityEls) * sizeof(columnType_t)) };
//			memoryOffset += pureBytesForColumn;
//		});
//
//		const auto prevNumOfEls{ table.numOfFilledEls };
//		table.numOfFilledEls += numOfElements; 
//		return get(table, prevNumOfEls, numOfElements);
//
//	}

	template <typename Types, typename ... ExtractedTypes>
	consteval auto createMutTablePack()
	{
		constexpr auto numOfTypes{ std::tuple_size_v<Types> };
		constexpr auto numOfExtractedTypes{ sizeof ... (ExtractedTypes) };
		if constexpr (numOfExtractedTypes == numOfTypes)
		{
			return std::tuple<Dod::MutTable<ExtractedTypes>...>{};
		}
		else
		{
			constexpr auto elementId{ numOfTypes - 1 - numOfExtractedTypes };
			using type_t = std::tuple_element_t<elementId, Types>;
			return createMutTablePack<Types, type_t, ExtractedTypes...>();
		}
	}

	template <typename Types, typename ... ExtractedTypes>
	consteval auto createImTablePack()
	{
		constexpr auto numOfTypes{ std::tuple_size_v<Types> };
		constexpr auto numOfExtractedTypes{ sizeof ... (ExtractedTypes) };
		if constexpr (numOfExtractedTypes == numOfTypes)
		{
			return std::tuple<Dod::ImTable<ExtractedTypes>...>{};
		}
		else
		{
			constexpr auto elementId{ numOfTypes - 1 - numOfExtractedTypes };
			using type_t = std::tuple_element_t<elementId, Types>;
			return createImTablePack<Types, type_t, ExtractedTypes...>();
		}
	}

	void destroyRange(CommonData::CDTable auto& table, int32_t beginElementId, int32_t endElementId) requires 
		CommonData::CTrivialTable<decltype(table)>
	{
		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		auto memPosition{ table.dataBegin };
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			const auto offset{ roundToCells(table.capacityEls * columnTypeSize) };
			memPosition += offset;
		});
	}

	void flush(CommonData::CDTable auto& table) noexcept requires CommonData::CTrivialTable<decltype(table)>
	{
		
		destroyRange(table, 0, table.numOfFilledEls);
		table.numOfFilledEls = 0;

	}

	[[nodiscard]] auto getCapacity(const CommonData::CTable auto& table) noexcept
	{
		return table.capacityEls;
	}

	void append(CommonData::CDTable auto& table, const CommonData::CImTable auto& srcTable) noexcept requires 
		CommonData::CTablesStructureIsSame<decltype(table), decltype(srcTable)> && 
		CommonData::CTrivialTable<decltype(srcTable)>
	{

		const auto destSpaceLeft{ table.capacityEls - getNumFilledElements(table) };
		const auto srcNumOfElements{ getNumFilledElements(srcTable) };
		const auto numOfElementsToAppend{ std::min(destSpaceLeft, srcNumOfElements) };

		const auto startElementPosition{ getNumFilledElements(table) };
		table.numOfFilledEls += numOfElementsToAppend;

		using destTableType_t = std::decay_t<decltype(table)>;
		using destTypes_t = destTableType_t::types_t;

		using srcTableType_t = std::decay_t<decltype(srcTable)>;
		using srcTypes_t = srcTableType_t::types_t;

		auto dstRowMemPosition{ table.dataBegin };
		auto srcRowMemPosition{ srcTable.dataBegin };

		constexpr auto numOfColumns{ std::tuple_size_v<destTypes_t> };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using destColumnType_t = std::tuple_element_t<currColId, destTypes_t>;
			using srcColumnType_t = std::tuple_element_t<currColId, srcTypes_t>;
			constexpr auto columnTypeSize{ sizeof(destColumnType_t) };
			const auto dstBegin{ dstRowMemPosition + startElementPosition * columnTypeSize };
			const auto srcBegin{ srcRowMemPosition };

			std::memcpy(dstBegin, srcBegin, columnTypeSize * numOfElementsToAppend);

			const auto destOffset{ roundToCells(table.capacityEls * columnTypeSize) };
			dstRowMemPosition += destOffset;
			const auto sourceOffset{ roundToCells(srcTable.capacityEls * columnTypeSize) };
			srcRowMemPosition += sourceOffset;
		});

	}

	void remove(CommonData::CDTable auto& table, CommonData::CMonoImTable auto indicesToRemove) noexcept requires 
		std::is_same_v<std::tuple_element_t<0, typename decltype(indicesToRemove)::types_t>, int32_t>
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		const auto totalElements{ getNumFilledElements(table) };
		auto columnDataBegin{ table.dataBegin };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			const auto columnTypedDataBegin{ reinterpret_cast<columnType_t*>(columnDataBegin) };
			auto targetIdx{ totalElements - 1 };
			for (int32_t idx{ Dod::DataUtils::getNumFilledElements(indicesToRemove) - 1 }; idx >= 0 ; --idx)
			{
				const auto removeId{ DataUtils::get(indicesToRemove, idx) };
				if constexpr (std::is_move_assignable_v<columnType_t>)
					columnTypedDataBegin[removeId] = std::move(columnTypedDataBegin[targetIdx]);
				else
					columnTypedDataBegin[removeId] = columnTypedDataBegin[targetIdx];
				--targetIdx;
			}
			const auto offset{ roundToCells(table.capacityEls * columnTypeSize) };
			columnDataBegin += offset;
		});
		const auto beginElementId{ table.numOfFilledEls - Dod::DataUtils::getNumFilledElements(indicesToRemove) };
		const auto endElementId{ table.numOfFilledEls };
		destroyRange(table, beginElementId, endElementId);
		table.numOfFilledEls -= Dod::DataUtils::getNumFilledElements(indicesToRemove);

	}

	[[nodiscard]] auto createGuidedImTable(CommonData::CMonoImTable auto srcTable, CommonData::CMonoImTable auto indices) noexcept requires
		std::is_same_v<std::tuple_element_t<0, typename decltype(indices)::types_t>, int32_t>
	{

		const auto bCanCreate{ Dod::DataUtils::getNumFilledElements(srcTable) == Dod::DataUtils::getNumFilledElements(indices) };

		using table_t = decltype(srcTable);
		using types_t = table_t::types_t;

		const auto createSorted = [] <typename ... T> (std::tuple<T...>) {
			return Dod::ImTableGuided<T...>{};
		};

		auto sortedTable{ createSorted(types_t{}) };
		if (!bCanCreate)
			return sortedTable;

		sortedTable.dataBegin = srcTable.dataBegin;
		sortedTable.capacityEls = srcTable.capacityEls;
		sortedTable.numOfElements = Dod::DataUtils::getNumFilledElements(srcTable);
		sortedTable.guid = indices;

		return sortedTable;

	}

};
