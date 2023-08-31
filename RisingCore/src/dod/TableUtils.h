#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include "Buffers.h"
#include "BufferUtils.h"

#include <type_traits>
#include <concepts>

namespace Dod::DataUtils
{

	template <typename Tuple, size_t index = 0>
	constexpr size_t computeTypesSize() {
		if constexpr (index < std::tuple_size_v<Tuple>) 
		{
			using type_t = std::tuple_element_t<index, Tuple>;
			return sizeof(type_t) + computeTypesSize<Tuple, index + 1>();
		}
		else
			return 0;
	}

	template <typename Types, size_t Index = 0>
	constexpr auto computeDeadbucketSizeInBytes() noexcept
	{
		if constexpr (Index < std::tuple_size_v<Types>)
		{
			using type_t = std::tuple_element_t<Index, Types>;
			const auto currentSize = sizeof(type_t);
			const auto remainingSize = computeDeadbucketSizeInBytes<Types, Index + 1>();
			return (currentSize > remainingSize) ? currentSize : remainingSize;
		}
		else
		{
			return 0;
		}
	}

	void initTableFromMemoryImpl(CommonData::CTable auto& dbTable, auto&& actualData) noexcept
	{

		using tableType_t = typename std::decay_t<decltype(dbTable)>;
		using types_t = tableType_t::types_t;

		constexpr auto rowSize{ computeTypesSize<types_t>() };

		dbTable.dataBegin = actualData.dataBegin;
		const auto capacityInBytes{ (actualData.dataEnd - actualData.dataBegin) };

		constexpr auto deadBucketSize{ computeDeadbucketSizeInBytes<types_t>() };
		const auto capacityInBytesWithDeadBuffer{ (capacityInBytes - deadBucketSize) * (capacityInBytes > deadBucketSize) };
		dbTable.capacityEls = static_cast<int32_t>(capacityInBytesWithDeadBuffer / rowSize);
		if constexpr (requires {dbTable.numOfFilledEls; })
			dbTable.numOfFilledEls = 0;

	}

	void initFromMemory(CommonData::CTable auto& dbTable, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, beginIndex, endIndex) };
		initTableFromMemoryImpl(dbTable, actualData);

	}

	void initFromMemory(CommonData::CTable auto& dbTable, const auto& memSpan) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, 0, static_cast<int32_t>(memSpan.dataEnd - memSpan.dataBegin)) };
		initTableFromMemoryImpl(dbTable, actualData);

	}

	[[nodiscard]] auto getNumFilledElements(CommonData::CTable auto table) noexcept
	{
		if constexpr (requires { table.numOfFilledEls; })
			return table.numOfFilledEls;
		else
			return static_cast<int32_t>(table.dataEnd - table.dataBegin);
	}

	void populate(CommonData::CTable auto& table, bool strobe, auto&& ... value) noexcept
	{

		const auto bCanAddValue{ (Dod::DataUtils::getNumFilledElements(table) + 1 <= table.capacityEls) && strobe };

		table.numOfFilledEls += size_t(1) * bCanAddValue;
		const auto elementPosition{ table.numOfFilledEls };

		using tableType_t = typename std::decay_t<decltype(table)>;
		using types_t = tableType_t::types_t;

		constexpr auto deadBucketSize{ computeDeadbucketSizeInBytes<types_t>() };

		size_t memoryOffset{ deadBucketSize };

		MemTypes::dataPoint_t rowMemPosition{ table.dataBegin };
		(std::invoke([&] {
			using valueType_t = std::decay_t<decltype(value)>;
			const auto inColumnMemoryOffset{ memoryOffset + (elementPosition - 1) * sizeof(valueType_t) };
			const auto memoryPosition{ reinterpret_cast<valueType_t*>( table.dataBegin + inColumnMemoryOffset * bCanAddValue) };
			*memoryPosition = std::move(value);
			memoryOffset += (table.capacityEls) * sizeof(valueType_t);
		}), ...);

	}

	template <std::size_t N, std::size_t Current = 0>
	constexpr void constexprLoop(auto func, auto&& ... args) {
		if constexpr (Current < N) {
			func.operator()<Current>(std::forward<decltype(args)>(args)...);
			constexprLoop<N, Current + 1>(func);
		}
	}

	template<int32_t collumnId>
	[[nodiscard]] auto get(const CommonData::CTable auto& table) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		MemTypes::dataPoint_t rowMemPosition{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		constexprLoop<collumnId>([&]<size_t currColId>() {
			constexpr auto collumnTypeSize{ sizeof(std::tuple_element_t<currColId, types_t>) };
			rowMemPosition += table.capacityEls * collumnTypeSize;
		});

		using returnType_t = std::tuple_element_t<static_cast<size_t>(collumnId), types_t>;
		constexpr auto returnTypeSize{ sizeof(returnType_t) };
		struct Span
		{
			MemTypes::dataPoint_t dataBegin{ rowMemPosition };
			MemTypes::dataPoint_t dataEnd{ rowMemPosition + table.numOfFilledEls * returnTypeSize };
		};

		Dod::ImBuffer<returnType_t> imBuffer;
		initFromMemory(imBuffer, Span{});

		return imBuffer;

	}

	template <typename Types, typename ... ExtractedTypes>
	constexpr auto createImBuffersPack()
	{
		constexpr auto numOfTypes{ std::tuple_size_v<Types> };
		constexpr auto numOfExtractedTypes{ sizeof ... (ExtractedTypes) };
		if constexpr (numOfExtractedTypes == numOfTypes)
		{
			return std::tuple<Dod::ImBuffer<ExtractedTypes>...>{};
		}
		else
		{
			constexpr auto elementId{ numOfTypes - 1 - numOfExtractedTypes };
			using type_t = std::tuple_element_t<elementId, Types>;
			return createImBuffersPack<Types, type_t, ExtractedTypes...>();
		}
	}

	template <typename Tuple, typename Func>
	constexpr void for_each_in_tuple(Tuple& tuple, Func&& func) {
		std::apply([&](auto&... args) {
			(func(args), ...);
		}, tuple);
	}

	[[nodiscard]] auto get(const CommonData::CTable auto& table) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		using returnType_t = decltype(createImBuffersPack<types_t>());
		returnType_t buffersPack;

		MemTypes::dataPoint_t rowMemPosition{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		for_each_in_tuple(buffersPack, [&](auto& bufferPack) {
			
			using pack_t = std::decay_t<decltype(bufferPack)>;
			using packType_t = pack_t::type_t;

			constexpr auto columnTypeSize{ sizeof(packType_t) };
			struct Span
			{
				MemTypes::dataPoint_t dataBegin{ nullptr };
				MemTypes::dataPoint_t dataEnd{ nullptr };
			};

			initFromMemory(bufferPack, Span(rowMemPosition, rowMemPosition + table.numOfFilledEls * columnTypeSize));
			rowMemPosition += table.capacityEls * columnTypeSize;

		});

		return buffersPack;

	}

	void flush(CommonData::CTable auto& table) noexcept
	{
		table.numOfFilledEls = 0;
	}

	[[nodiscard]] auto getCapacity(const CommonData::CTable auto& table) noexcept
	{
		return table.capacityEls;
	}

	void append(CommonData::CTable auto& table, const CommonData::CTable auto& srcTable) noexcept
	{

		const auto destSpaceLeft{ table.capacityEls - getNumFilledElements(table) };
		const auto srcNumOfElements{ getNumFilledElements(srcTable) };
		const auto numOfElementsToAppend{ std::min(destSpaceLeft, srcNumOfElements) };

		const auto startElementPosition{ getNumFilledElements(table) };
		table.numOfFilledEls += numOfElementsToAppend;

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		auto dstRowMemPosition{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		auto srcRowMemPosition{ srcTable.dataBegin + computeDeadbucketSizeInBytes<types_t>() };

		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		constexprLoop<numOfColumns>([&]<size_t currColId>() {
			constexpr auto columnTypeSize{ sizeof(std::tuple_element_t<currColId, types_t>) };
			const auto dstBegin{ dstRowMemPosition + startElementPosition * columnTypeSize };
			const auto srcBegin{ srcRowMemPosition };
			std::memcpy(dstBegin, srcBegin, columnTypeSize * numOfElementsToAppend);
			dstRowMemPosition += table.capacityEls * columnTypeSize;
			srcRowMemPosition += srcTable.capacityEls * columnTypeSize;
		});

	}

	void remove(CommonData::CTable auto& table, const ImBuffer<int32_t> indicesToRemove) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		const auto totalElements{ getNumFilledElements(table) };
		auto columnDataBegin{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			const auto columnTypedDataBegin{ reinterpret_cast<columnType_t*>(columnDataBegin) };
			auto targetIdx{ totalElements - 1 };
			for (int32_t idx{ Dod::DataUtils::getNumFilledElements(indicesToRemove) - 1 }; idx >= 0 ; --idx)
			{
				const auto removeId{ indicesToRemove.dataBegin[idx] };
				columnTypedDataBegin[removeId] = columnTypedDataBegin[targetIdx];
				--targetIdx;
			}
			columnDataBegin += table.capacityEls * columnTypeSize;
		});
		table.numOfFilledEls -= Dod::DataUtils::getNumFilledElements(indicesToRemove);

	}

};
