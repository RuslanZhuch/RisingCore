#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include "Buffers.h"
#include "Tables.h"

#include <type_traits>
#include <concepts>

namespace Dod::DataUtils
{

	void initBufferFromMemoryImpl(CommonData::CBuffer auto& dbBuffer, auto&& actualData) noexcept
	{
		//TODO: Is it legal?
		dbBuffer.dataBegin = reinterpret_cast<decltype(dbBuffer.dataBegin)>(actualData.dataBegin);
		dbBuffer.dataEnd = reinterpret_cast<decltype(dbBuffer.dataEnd)>(actualData.dataEnd);
		if constexpr (requires {dbBuffer.numOfFilledEls; })
			dbBuffer.numOfFilledEls = 0;
	}

	void initFromMemory(CommonData::CBuffer auto& dbBuffer, const auto& memSpan) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, 0, static_cast<int32_t>(memSpan.dataEnd - memSpan.dataBegin)) };
		initBufferFromMemoryImpl(dbBuffer, actualData);

	}

	void initFromMemory(CommonData::CBuffer auto& dbBuffer, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, beginIndex, endIndex) };
		initBufferFromMemoryImpl(dbBuffer, actualData);

	}

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

	template <typename Types, std::size_t Index = 0>
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

	template <typename T>
	[[nodiscard]] static auto initFromArray(DBBuffer<T>& dbBuffer, auto& src) noexcept
	{

		struct MemSpan
		{
			MemTypes::dataPoint_t dataBegin{};
			MemTypes::dataPoint_t dataEnd{};
		};
		const MemSpan memSpan(
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()),
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data() + src.size())
		);

		initFromMemory(dbBuffer, memSpan);

	}

	template <typename T>
	[[nodiscard]] static auto initFromArray(ImBuffer<T>& imBuffer, const auto& src) noexcept
	{

		struct MemSpan
		{
			MemTypes::dataConstPoint_t dataBegin{};
			MemTypes::dataConstPoint_t dataEnd{};
		};
		const MemSpan memSpan(
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(src.data()),
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(src.data() + src.size())
		);

		initFromMemory(imBuffer, memSpan);

	}

	template <typename T>
	[[nodiscard]] static auto initFromArray(MutBuffer<T>& buffer, auto& src) noexcept
	{

		struct MemSpan
		{
			MemTypes::dataPoint_t dataBegin{};
			MemTypes::dataPoint_t dataEnd{};
		};
		const MemSpan memSpan(
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()),
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data() + src.size())
		);

		initFromMemory(buffer, memSpan);

	}

	[[nodiscard]] auto getNumFilledElements(CommonData::CTable auto table) noexcept
	{
		if constexpr (requires { table.numOfFilledEls; })
			return table.numOfFilledEls;
		else
			return static_cast<int32_t>(table.dataEnd - table.dataBegin);
	}

	template<CommonData::CBuffer BufferType>
	[[nodiscard]] auto getNumFilledElements(BufferType buffer) noexcept
	{
		if constexpr (requires { buffer.numOfFilledEls; })
			return buffer.numOfFilledEls;
		else 
			return static_cast<int32_t>(buffer.dataEnd - buffer.dataBegin);
	}

	template<typename T>
	void constructBack(DBBuffer<T>& buffer, bool strobe = true) noexcept
	{

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (Dod::DataUtils::getNumFilledElements(buffer) + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		std::construct_at<T>(buffer.dataBegin + Dod::DataUtils::getNumFilledElements(buffer) * bCanAddValue);

	}

	template<typename T>
	void constructBack(DBBuffer<T>& buffer, const T& copy, bool strobe = true) noexcept
	{

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (Dod::DataUtils::getNumFilledElements(buffer) + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		buffer.dataBegin[Dod::DataUtils::getNumFilledElements(buffer) * bCanAddValue] = copy;

	}

	template<typename T>
	void emplaceBack(DBBuffer<T>& buffer, T&& element, bool strobe = true) noexcept
	{

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (Dod::DataUtils::getNumFilledElements(buffer) + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		buffer.dataBegin[Dod::DataUtils::getNumFilledElements(buffer) * bCanAddValue] = std::move(element);

	}

	template<typename T>
	void populate(DBBuffer<T>& buffer, T value, bool strobe) noexcept 
		requires requires() { requires std::is_trivially_constructible_v<T>; }
	{

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (Dod::DataUtils::getNumFilledElements(buffer) + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		buffer.dataBegin[Dod::DataUtils::getNumFilledElements(buffer) * bCanAddValue] = std::move(value);

	}

	template<typename T>
	void populate(DBBuffer<T>& buffer, T value, bool strobe) noexcept
	{

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (Dod::DataUtils::getNumFilledElements(buffer) + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		const auto position{ Dod::DataUtils::getNumFilledElements(buffer) * bCanAddValue };
		std::construct_at<T>(buffer.dataBegin + position);
		buffer.dataBegin[position] = std::move(value);

	}

	template<typename ... T>
	void populate(DBTable<T...>& table, bool strobe, T&& ... value) noexcept
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

	template<typename T>
	[[nodiscard]] T& get(const DBBuffer<T>& buffer, int32_t elId) noexcept
	{
		return buffer.dataBegin[elId + 1];
	}

	[[nodiscard]] auto& get(const auto& buffer, int32_t elId) noexcept
	{
		return buffer.dataBegin[elId];
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

//	template <typename Types>
//	struct ImBuffersPack {
//		using type_t = decltype(createImBuffersPack<Types>());
//	};

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

	template <typename BufferType>
	[[nodiscard]] auto& get(const BufferType& buffer, int32_t elId) noexcept requires requires(BufferType buff) { 
		buff.sortingDataBegin;
		buff.sortingDataEnd;	
	}
	{
		return buffer.dataBegin[buffer.sortingDataBegin[elId]];
	}

	template<typename T>
	void remove(DBBuffer<T>& buffer, const ImBuffer<int32_t> indicesToRemove) noexcept
	{

		auto targetIdx{ Dod::DataUtils::getNumFilledElements(buffer) };
		for (int32_t idx{ 0 }; idx < Dod::DataUtils::getNumFilledElements(indicesToRemove); ++idx)
		{
			const auto removeId{ indicesToRemove.dataBegin[idx] };
			std::swap(buffer.dataBegin[removeId + 1], buffer.dataBegin[targetIdx]);
			--targetIdx;
		}
		buffer.numOfFilledEls -= Dod::DataUtils::getNumFilledElements(indicesToRemove);

	}

	template<typename BufferType>
	void initFromBuffer(BufferType& dest, const DBBuffer<typename BufferType::type_t>& src) noexcept
	{
		dest.dataBegin = src.dataBegin + 1;
		dest.dataEnd = src.dataEnd;
	}

	template<typename BufferType>
	void initFromBuffer(BufferType& dest, const DBBuffer<typename BufferType::type_t>& src, int32_t elementBeginId, int32_t elementEndId) noexcept
	{
		dest.dataBegin = src.dataBegin + 1 + elementBeginId;
		dest.dataEnd = src.dataBegin + 1 + elementEndId;
	}

	[[nodiscard]] auto createImFromBuffer(const auto& srcBuffer) noexcept
	{
	
		using type_t = std::remove_pointer_t<decltype(srcBuffer.dataBegin)>;

		Dod::ImBuffer<type_t> imBuffer;
		initFromBuffer(imBuffer, srcBuffer, 0, Dod::DataUtils::getNumFilledElements(srcBuffer));

		return imBuffer;

	}	
	
	template <typename T>
	[[nodiscard]] auto createSortedImBuffer(ImBuffer<T> srcBuffer, ImBuffer<int32_t> indices) noexcept
	{
	
		const auto bCanCreate{ Dod::DataUtils::getNumFilledElements(srcBuffer) == Dod::DataUtils::getNumFilledElements(indices) };

		Dod::SortedImBuffer<T> sortedBuffer;
		if (!bCanCreate)
			return sortedBuffer;

		sortedBuffer.dataBegin = srcBuffer.dataBegin;
		sortedBuffer.dataEnd = srcBuffer.dataEnd;
		sortedBuffer.sortingDataBegin = indices.dataBegin;
		sortedBuffer.sortingDataEnd = indices.dataEnd;
		
		return sortedBuffer;

	}

	template<typename BufferType>
	[[nodiscard]] auto getBufferBounds(const BufferType& buffer) noexcept
	{

		struct Output
		{
			decltype(buffer.dataBegin) dataBegin{};
			decltype(buffer.dataEnd) dataEnd{};
		};

		using type_t = std::remove_pointer_t<decltype(buffer.dataBegin)>;
		if constexpr (std::is_same_v<BufferType, DBBuffer<type_t>>)
			return Output(buffer.dataBegin + 1, buffer.dataEnd);
		else 
			return Output(buffer.dataBegin, buffer.dataEnd);

	}

	template<typename T>
	void flush(DBBuffer<T>& dest) noexcept
	{
		dest.numOfFilledEls = 0;
	}

	void flush(CommonData::CTable auto& table) noexcept
	{
		table.numOfFilledEls = 0;
	}

	template<typename T>
	[[nodiscard]] auto getCapacity(const DBBuffer<T>& dest) noexcept
	{
		return static_cast<int32_t>(dest.dataEnd - dest.dataBegin - 1);
	}

	template<typename T>
	void append(DBBuffer<T>& dest, Dod::ImBuffer<T> src) noexcept 
		requires requires() { requires std::is_trivially_copyable_v<T>; }
	{
		const auto spaceLeft{ Dod::DataUtils::getCapacity(dest) - Dod::DataUtils::getNumFilledElements(dest) };
		const auto allowToFill{ std::max(spaceLeft, 0) };
		const auto numOfElements{ std::min(allowToFill, Dod::DataUtils::getNumFilledElements(src)) };
		const auto begin{ dest.dataBegin + 1 + Dod::DataUtils::getNumFilledElements(dest) };
		std::memcpy(begin, src.dataBegin, sizeof(T) * numOfElements);
		dest.numOfFilledEls += numOfElements;
	}

	template<typename T>
	void append(DBBuffer<T>& dest, Dod::ImBuffer<T> src) noexcept
	{
		const auto srcNumOfElements{ Dod::DataUtils::getNumFilledElements(src) };
		for (int32_t elId{}; elId < srcNumOfElements; ++elId)
		{
			Dod::DataUtils::populate(dest, Dod::DataUtils::get(src, elId), true);
		}
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
		static_assert(false, "In progress");
		auto targetIdx{ Dod::DataUtils::getNumFilledElements(buffer) };
		for (int32_t idx{ 0 }; idx < Dod::DataUtils::getNumFilledElements(indicesToRemove); ++idx)
		{
			const auto removeId{ indicesToRemove.dataBegin[idx] };
			std::swap(buffer.dataBegin[removeId + 1], buffer.dataBegin[targetIdx]);
			--targetIdx;
		}
		buffer.numOfFilledEls -= Dod::DataUtils::getNumFilledElements(indicesToRemove);

	}

};
