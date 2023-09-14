#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include "Buffers.h"
#include "CommonData.h"

#include <type_traits>
#include <concepts>

namespace Dod::DataUtils
{

//	void initBufferFromMemoryImpl(CommonData::CBuffer auto& dbBuffer, auto&& actualData) noexcept
//	{
//		//TODO: Is it legal?
//		dbBuffer.dataBegin = reinterpret_cast<decltype(dbBuffer.dataBegin)>(actualData.dataBegin);
//		dbBuffer.dataEnd = reinterpret_cast<decltype(dbBuffer.dataEnd)>(actualData.dataEnd);
//		if constexpr (requires {dbBuffer.numOfFilledEls; })
//			dbBuffer.numOfFilledEls = 0;
//	}

	void initFromMemory(CommonData::CBuffer auto& dbBuffer, const auto& memSpan) noexcept
	{

		dbBuffer.dataBegin = reinterpret_cast<decltype(dbBuffer.dataBegin)>(memSpan.dataBegin);
		dbBuffer.dataEnd = reinterpret_cast<decltype(dbBuffer.dataEnd)>(memSpan.dataEnd);
		if constexpr (requires {dbBuffer.numOfFilledEls; })
			dbBuffer.numOfFilledEls = 0;

//		const auto actualData{ MemUtils::acquire(memSpan, 0, static_cast<int32_t>(memSpan.dataEnd - memSpan.dataBegin), 1) };
//		initBufferFromMemoryImpl(dbBuffer, actualData);

	}

//	void initFromMemory(CommonData::CBuffer auto& dbBuffer, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
//	{
//
//		const auto actualData{ MemUtils::acquire(memSpan, beginIndex, endIndex, 1) };
//		initBufferFromMemoryImpl(dbBuffer, actualData);
//
//	}

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

		const auto capacity{ static_cast<int32_t>(buffer.dataEnd - buffer.dataBegin) };
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

	template<typename T>
	[[nodiscard]] T& get(const DBBuffer<T>& buffer, int32_t elId) noexcept
	{
		return buffer.dataBegin[elId + 1];
	}

	[[nodiscard]] auto& get(const auto& buffer, int32_t elId) noexcept
	{
		return buffer.dataBegin[elId];
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

};
