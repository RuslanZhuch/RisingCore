#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include "Buffers.h"
#include "BufferUtils.h"

#include "Helpers.h"

#include <type_traits>
#include <concepts>

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

	template <typename Types, size_t Index = 0>
	[[nodiscard]] consteval auto computeDeadbucketSizeInBytes() noexcept
	{
		if constexpr (Index < std::tuple_size_v<Types>)
		{
			using type_t = std::tuple_element_t<Index, Types>;
			constexpr auto currentSize = static_cast<MemTypes::capacity_t>(sizeof(type_t));
			constexpr auto remainingSize = computeDeadbucketSizeInBytes<Types, Index + 1>();
			return (currentSize > remainingSize) ? currentSize : remainingSize;
		}
		else
			return 0;
	}

	template <typename Type>
	[[nodiscard]] constexpr auto computeTypeOffset(const Type* ptr) noexcept
	{
		constexpr auto alignment{ alignof(Type) };
		const auto address{ reinterpret_cast<std::uintptr_t>(ptr) };
		return address % alignment;
	}

	void initTableFromMemoryImpl(CommonData::CTable auto& dbTable, int32_t numOfElements, auto&& actualData) noexcept
	{

		using tableType_t = typename std::decay_t<decltype(dbTable)>;
		using types_t = tableType_t::types_t;

		constexpr auto rowSize{ computeTypesSize<types_t>() };

		dbTable.dataBegin = actualData.dataBegin;

		constexpr auto deadBucketSize{ computeDeadbucketSizeInBytes<types_t>() };

		MemTypes::capacity_t needBytes{ deadBucketSize };
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t columnId>() {
			using type_t = std::tuple_element_t<columnId, types_t>;
			const auto alignmentOffset{ static_cast<MemTypes::capacity_t>(MemUtils::getAlignOffset(actualData.dataBegin + needBytes, alignof(type_t))) };
			const auto pureBytesForColumn{ static_cast<MemTypes::capacity_t>(sizeof(type_t) * numOfElements) };
			needBytes += alignmentOffset + pureBytesForColumn;
		});

		const auto capacityInBytes{ static_cast<MemTypes::capacity_t>(actualData.dataEnd - actualData.dataBegin) };
		dbTable.capacityEls = numOfElements * (needBytes <= capacityInBytes);

		if constexpr (requires { dbTable.numOfFilledEls; })
			dbTable.numOfFilledEls = 0;

	}

//	void initFromMemory(CommonData::CTable auto& dbTable, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t numOfBytes) noexcept
//	{
//
//		const auto actualData{ MemUtils::acquire(memSpan, beginIndex, numOfBytes, 1) };
//		initTableFromMemoryImpl(dbTable, actualData);
//
//	}

	void initFromMemory(CommonData::CTable auto& dbTable, int32_t numOfElements, const auto& memSpan) noexcept
	{

//		const auto actualData{ MemUtils::acquire(memSpan, 0, static_cast<int32_t>(memSpan.dataEnd - memSpan.dataBegin), 1) };
		initTableFromMemoryImpl(dbTable, numOfElements, memSpan);

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
			const auto alignmentOffset{ MemUtils::getAlignOffset(table.dataBegin + memoryOffset, alignof(valueType_t)) };
			const auto inColumnMemoryOffset{ memoryOffset + alignmentOffset + (elementPosition - 1) * sizeof(valueType_t) };
			const auto memoryPosition{ reinterpret_cast<valueType_t*>(table.dataBegin + inColumnMemoryOffset * bCanAddValue) };
			if constexpr (std::is_trivial_v<valueType_t>)
			{
				*memoryPosition = std::forward<decltype(value)>(value);
			}
			else 
			{
				if (bCanAddValue)
					std::construct_at<valueType_t>(memoryPosition, std::forward<decltype(value)>(value));
			}
			memoryOffset += (table.capacityEls) * sizeof(valueType_t);
		}), ...);

	}

	template<int32_t collumnId>
	[[nodiscard]] auto get(const CommonData::CTable auto& table) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		MemTypes::dataPoint_t rowMemPosition{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		RisingCore::Helpers::constexprLoop<collumnId>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			const auto alignmentOffset{ MemUtils::getAlignOffset(rowMemPosition, alignof(columnType_t)) };
			rowMemPosition += alignmentOffset + table.capacityEls * columnTypeSize;
		});


		using returnType_t = std::tuple_element_t<static_cast<size_t>(collumnId), types_t>;
		rowMemPosition += MemUtils::getAlignOffset(rowMemPosition, alignof(returnType_t));

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
	consteval auto createImBuffersPack()
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

	[[nodiscard]] auto get(const CommonData::CTable auto& table) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		using returnType_t = decltype(createImBuffersPack<types_t>());
		returnType_t buffersPack;

		MemTypes::dataPoint_t rowMemPosition{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		RisingCore::Helpers::constexprLoop<std::tuple_size_v<returnType_t>>([&]<size_t columnId>() {
			auto& bufferPack{ std::get<columnId>(buffersPack) };
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

	void destroyRange(CommonData::CTable auto& table, int32_t beginElementId, int32_t endElementId)
	{
		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;

		auto memPosition{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			if constexpr (std::is_trivial_v<columnType_t> == false)
			{
				memPosition += MemUtils::getAlignOffset(memPosition, alignof(columnType_t));
				const auto destructBegin{ memPosition };
				for (int32_t elId{ beginElementId }; elId < endElementId; ++elId)
				{
					const auto destructPoint = reinterpret_cast<columnType_t*>(memPosition) + elId;
					std::destroy_at(destructPoint);
				}
			}
			memPosition += table.capacityEls * columnTypeSize;
		});
	}

	void flush(CommonData::CTable auto& table) noexcept
	{
		
		destroyRange(table, 0, table.numOfFilledEls);
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
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			const auto dstAlignmentOffset{ MemUtils::getAlignOffset(dstRowMemPosition, alignof(columnType_t)) };
			const auto dstBegin{ dstRowMemPosition + dstAlignmentOffset + startElementPosition * columnTypeSize };
			const auto srcAlignmentOffset{ MemUtils::getAlignOffset(srcRowMemPosition, alignof(columnType_t)) };
			const auto srcBegin{ srcRowMemPosition + srcAlignmentOffset };
			if constexpr(std::is_trivial_v<columnType_t>)
				std::memcpy(dstBegin, srcBegin, columnTypeSize * numOfElementsToAppend);
			else
			{
				for (int32_t elId{}; elId < numOfElementsToAppend; ++elId)
				{
					const auto constructPoint = reinterpret_cast<columnType_t*>(dstBegin) + elId;
					auto&& srcValue{ reinterpret_cast<columnType_t*>(srcBegin) + elId };
					std::construct_at<columnType_t>(constructPoint, std::forward<columnType_t>(*srcValue));
				}
			}
			dstRowMemPosition += dstAlignmentOffset + table.capacityEls * columnTypeSize;
			srcRowMemPosition += srcAlignmentOffset + srcTable.capacityEls * columnTypeSize;
		});

	}

	void remove(CommonData::CTable auto& table, const ImBuffer<int32_t> indicesToRemove) noexcept
	{

		using tableType = std::decay_t<decltype(table)>;
		using types_t = tableType::types_t;
		constexpr auto numOfColumns{ std::tuple_size_v<types_t> };
		const auto totalElements{ getNumFilledElements(table) };
		auto columnDataBegin{ table.dataBegin + computeDeadbucketSizeInBytes<types_t>() };
		RisingCore::Helpers::constexprLoop<numOfColumns>([&]<size_t currColId>() {
			using columnType_t = std::tuple_element_t<currColId, types_t>;
			constexpr auto columnTypeSize{ sizeof(columnType_t) };
			const auto alignmentOffset{ MemUtils::getAlignOffset(columnDataBegin, alignof(columnType_t)) };
			const auto columnTypedDataBegin{ reinterpret_cast<columnType_t*>(columnDataBegin + alignmentOffset) };
			auto targetIdx{ totalElements - 1 };
			for (int32_t idx{ Dod::DataUtils::getNumFilledElements(indicesToRemove) - 1 }; idx >= 0 ; --idx)
			{
				const auto removeId{ indicesToRemove.dataBegin[idx] };
				if constexpr (std::is_move_assignable_v<columnType_t>)
					columnTypedDataBegin[removeId] = std::move(columnTypedDataBegin[targetIdx]);
				else 
					columnTypedDataBegin[removeId] = columnTypedDataBegin[targetIdx];
				--targetIdx;
			}
			columnDataBegin += alignmentOffset + table.capacityEls * columnTypeSize;
		});
		const auto beginElementId{ table.numOfFilledEls - Dod::DataUtils::getNumFilledElements(indicesToRemove) };
		const auto endElementId{ table.numOfFilledEls };
		destroyRange(table, beginElementId, endElementId);
		table.numOfFilledEls -= Dod::DataUtils::getNumFilledElements(indicesToRemove);

	}

};
