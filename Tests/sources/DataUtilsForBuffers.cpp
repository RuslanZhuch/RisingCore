#include <gtest/gtest.h>

#include <dod/MemTypes.h>
#include <dod/Buffers.h>
#include <dod/DataUtils.h>
#include <dod/Algorithms.h>
#include "utils/CommonStructures.h"

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)
#include <ranges>

template <typename T>
struct CommonBuffer : Dod::CommonData::Buffer
{
	using type_t = typename T;
	const T* dataBegin{ nullptr };
	const T* dataEnd{ nullptr };
};

TEST(DataUtils, Initialization)
{

	constexpr auto dataSize{ 32 };
	std::array<Dod::MemTypes::data_t, dataSize> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	{
		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 8);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		MemorySpan memSpan2(memory.data() + 2, memory.data() + 8);

		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 6 };
		Dod::DataUtils::initFromMemory(buffer, memSpan2);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan2.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan2.dataBegin + endIndex);
	}

}


TEST(DBBufferUtils, Initialization)
{

	std::array<Dod::MemTypes::data_t, 32> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		MemorySpan memSpan2(memory.data() + 2, memory.data() + 8);

		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 6 };
		Dod::DataUtils::initFromMemory(buffer, memSpan2);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan2.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan2.dataBegin + endIndex);
	}

}

TEST(DataUtils, InitFromArray)
{

	{
		std::array<Dod::MemTypes::data_t, 1024> memory;
		Dod::DBBuffer<Dod::MemTypes::data_t> buffer;
		Dod::DataUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, memory.data());
		EXPECT_EQ(buffer.dataEnd, memory.data() + memory.size());
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);
	}
	{
		std::array<Dod::MemTypes::data_t, 1024> memory;
		Dod::ImBuffer<Dod::MemTypes::data_t> buffer;
		Dod::DataUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, memory.data());
		EXPECT_EQ(buffer.dataEnd, memory.data() + memory.size());
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), memory.size());
	}
	{
		std::array<Dod::MemTypes::data_t, 1024> memory;
		Dod::MutBuffer<Dod::MemTypes::data_t> buffer;
		Dod::DataUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, memory.data());
		EXPECT_EQ(buffer.dataEnd, memory.data() + memory.size());
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), memory.size());
	}

	{
		std::array<int32_t, 128> memory;
		Dod::DBBuffer<Dod::MemTypes::data_t> buffer;
		Dod::DataUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data()));
		EXPECT_EQ(buffer.dataEnd, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data() + memory.size()));
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);
	}
	{
		std::array<int32_t, 128> memory;
		Dod::ImBuffer<Dod::MemTypes::data_t> buffer;
		Dod::DataUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data()));
		EXPECT_EQ(buffer.dataEnd, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data() + memory.size()));
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), memory.size() * sizeof(int32_t));
	}

}

TEST(DBBufferUtils, Population)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	{
		const int32_t value{ 1 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	}
	{
		const int32_t value{ 2 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}
	{
		const int32_t value{ 3 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 3), 3);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		const int32_t value{ 4 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 0), 4);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 3), 3);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}

}

TEST(DBBufferUtils, PopulationComplex)
{

	std::array<Dod::MemTypes::data_t, 128> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	using type_t = std::vector<int32_t>;

	Dod::DBBuffer<type_t> buffer;
	Dod::DataUtils::initFromMemory(buffer, memSpan);

	const type_t v1{ 1, 2, 3, 4, 5 };
	const type_t v2{ 5, 4, 3, 2, 1 };
	const type_t v3{ 6, 7, 8, 9, 4 };
	const type_t v4{ 4, 9, 7, 8, 6 };
	const type_t v5{ 2, 1, 4, 5, 3 };

	{
		Dod::DataUtils::populate(buffer, v1, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::DataUtils::populate(buffer, v2, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::DataUtils::populate(buffer, v3, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), v3);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::populate(buffer, v4, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), v3);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::populate(buffer, v5, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), v3);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}

}

TEST(DBBufferUtils, ConstructBack)
{

	class Type
	{
	public:
		Type()
			: value(42)
		{}

		int32_t value{};
	};
	using type_t = Type;

	std::array<Dod::MemTypes::data_t, sizeof(type_t) * 5> memory{};
	Dod::DBBuffer<type_t> buffer;

	Dod::DataUtils::initFromArray(buffer, memory);

	{
		Dod::DataUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1).value, 0);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2).value, 0);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::DataUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2).value, 0);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::DataUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::constructBack(buffer, false);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3).value, 42);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::DataUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3).value, 42);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}

}

TEST(DBBufferUtils, ConstructBackCopy)
{

	class Type
	{
	public:
		Type()
			: 
			value(42),
			buffer({1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
		{}

		Type(int32_t v, const std::vector<float> buff)
			:
			value(v),
			buffer(std::move(buff))
		{}

		std::vector<float> buffer;
		int32_t value{};

		[[nodiscard]] auto operator<=>(const Type& other) const = default;

	};
	using type_t = Type;

	std::array<Dod::MemTypes::data_t, sizeof(type_t) * 5> memory{};
	Dod::DBBuffer<type_t> buffer;

	Dod::DataUtils::initFromArray(buffer, memory);

	const Type typeReal1(123, { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 });
	const Type typeReal2(456, { 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45 });
	const Type typeReal3(789, { 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65 });
	const Type typeReal4(753, { 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85 });
	const Type typeEmpty(0, {});

	{
		Dod::DataUtils::constructBack(buffer, typeReal1, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::DataUtils::constructBack(buffer, typeReal2, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::DataUtils::constructBack(buffer, typeReal3, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::constructBack(buffer, false);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::constructBack(buffer, typeReal4, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::DataUtils::constructBack(buffer, typeReal4, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::DataUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}
}

TEST(DBBufferUtils, EmplaceBack)
{

	class Type
	{
	public:
		Type()
			:
			value(42),
			buffer({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 })
		{}

		Type(int32_t v, const std::vector<float> buff)
			:
			value(v),
			buffer(std::move(buff))
		{}

		Type(const Type&) = delete;
		Type& operator=(const Type&) = delete;

		Type(Type&&) = default;
		Type& operator=(Type&&) = default;

		std::vector<float> buffer;
		int32_t value{};

		[[nodiscard]] auto operator<=>(const Type& other) const = default;

	};
	using type_t = Type;

	std::array<Dod::MemTypes::data_t, sizeof(type_t) * 5> memory{};
	Dod::DBBuffer<type_t> buffer;

	Dod::DataUtils::initFromArray(buffer, memory);

	const std::vector<float> srcBuffer1{ {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25} };
	const std::vector<float> srcBuffer2{ {31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45} };
	const std::vector<float> srcBuffer3{ {51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65} };
	const std::vector<float> srcBuffer4{ {71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85} };
	const std::vector<float> srcBuffer5;

	const Type typeReal1(123, srcBuffer1);
	const Type typeReal2(456, srcBuffer2);
	const Type typeReal3(789, srcBuffer3);
	const Type typeReal4(753, srcBuffer4);
	const Type typeEmpty(0, srcBuffer5);

	const auto moveReal1 = [&]() { return Type(123, srcBuffer1); };
	const auto moveReal2 = [&]() { return Type(456, srcBuffer2); };
	const auto moveReal3 = [&]() { return Type(789, srcBuffer3); };
	const auto moveReal4 = [&]() { return Type(753, srcBuffer4); };

	{
		Dod::DataUtils::emplaceBack(buffer, moveReal1(), true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::DataUtils::emplaceBack(buffer, moveReal2(), true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::DataUtils::emplaceBack(buffer, moveReal3(), true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::emplaceBack(buffer, moveReal4(), false);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::DataUtils::emplaceBack(buffer, moveReal4(), true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::DataUtils::emplaceBack(buffer, moveReal4(), true);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	}
}

TEST(DataUtils, DBBufferBound)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	using type_t = int32_t;
	Dod::DBBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	const auto boundSpan{ Dod::DataUtils::getBufferBounds(buffer) };
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(boundSpan.dataBegin), memSpan.dataBegin + sizeof(type_t));
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(boundSpan.dataEnd), memSpan.dataEnd);

}

TEST(DataUtils, BufferBound)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	using type_t = int32_t;
	CommonBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	const auto boundSpan{ Dod::DataUtils::getBufferBounds(buffer) };
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(boundSpan.dataBegin), memSpan.dataBegin);
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(boundSpan.dataEnd), memSpan.dataEnd);

}

TEST(DataUtils, InitializationFromDBBuffer)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());
	Dod::DBBuffer<int32_t> srcBuffer;

	Dod::DataUtils::initFromMemory(srcBuffer, memSpan);

	for (const auto value : std::to_array<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8 }))
		Dod::DataUtils::populate(srcBuffer, value, true);

	{
		CommonBuffer<int32_t> imBuffer;

		Dod::DataUtils::initFromBuffer(imBuffer, srcBuffer);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(imBuffer), Dod::DataUtils::getNumFilledElements(srcBuffer));
	}
	{
		CommonBuffer<int32_t> imBuffer;

		Dod::DataUtils::initFromBuffer(imBuffer, srcBuffer, 0, Dod::DataUtils::getNumFilledElements(srcBuffer));
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(imBuffer), Dod::DataUtils::getNumFilledElements(srcBuffer));
	}
	{
		CommonBuffer<int32_t> imBuffer;

		Dod::DataUtils::initFromBuffer(imBuffer, srcBuffer, 0, 4);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataBegin + 5);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(imBuffer), 4);
	}
	{
		CommonBuffer<int32_t> imBuffer;

		Dod::DataUtils::initFromBuffer(imBuffer, srcBuffer, 2, 5);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 3);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataBegin + 6);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(imBuffer), 3);
	}

}

TEST(DataUtils, InitializationFromMutBuffer)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());
	Dod::MutBuffer<int32_t> srcBuffer;

	Dod::DataUtils::initFromMemory(srcBuffer, memSpan);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(srcBuffer), 8);

	for (int32_t id{}; id < Dod::DataUtils::getNumFilledElements(srcBuffer); ++id)
		Dod::DataUtils::get(srcBuffer, id) = id + 1;

	{
		CommonBuffer<int32_t> imBuffer;

		Dod::DataUtils::initFromBuffer(imBuffer, srcBuffer);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(imBuffer), Dod::DataUtils::getNumFilledElements(srcBuffer));
	}

}

TEST(DataUtils, CreateImFromBuffer)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());
	Dod::DBBuffer<int32_t> srcBuffer;
	Dod::DataUtils::initFromMemory(srcBuffer, memSpan);

	for (const auto value : std::to_array<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8 }))
		Dod::DataUtils::populate(srcBuffer, value, true);

	const auto imBuffer{ Dod::DataUtils::createImFromBuffer(srcBuffer) };
	{
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(imBuffer), Dod::DataUtils::getNumFilledElements(srcBuffer));
	}

}

TEST(DBBufferUtils, GetValue)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
	std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;
	std::memcpy(memory.data(), values.data(), memory.size());

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	EXPECT_EQ(Dod::DataUtils::get(buffer, 0), static_cast<type_t>(2));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 1), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 2), static_cast<type_t>(4));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 3), static_cast<type_t>(5));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 4), static_cast<type_t>(6));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 5), static_cast<type_t>(7));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 6), static_cast<type_t>(8));

	Dod::DataUtils::get(buffer, 6) = 42;
	EXPECT_EQ(Dod::DataUtils::get(buffer, 6), static_cast<type_t>(42));

}

TEST(DataUtils, GetValue)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
	std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;
	std::memcpy(memory.data(), values.data(), memory.size());

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	CommonBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 8);
	EXPECT_EQ(Dod::DataUtils::get(buffer, 0), static_cast<type_t>(1));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 1), static_cast<type_t>(2));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 2), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 3), static_cast<type_t>(4));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 4), static_cast<type_t>(5));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 5), static_cast<type_t>(6));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 6), static_cast<type_t>(7));
	EXPECT_EQ(Dod::DataUtils::get(buffer, 7), static_cast<type_t>(8));
			
}

TEST(DataUtils, GetValueSorted)
{

	using type_t = int32_t;
	std::array<type_t, 8> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	Dod::ImBuffer<int32_t> buffer;
	Dod::DataUtils::initFromArray(buffer, values);

	std::array<type_t, values.size()> indices{{7, 6, 5, 4, 3, 2, 1, 0}};
	Dod::ImBuffer<int32_t> indicesBuffer;
	Dod::DataUtils::initFromArray(indicesBuffer, indices);

	Dod::SortedImBuffer<int32_t> sortedBuffer{ Dod::DataUtils::createSortedImBuffer(buffer, indicesBuffer) };

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(sortedBuffer), 8);
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 0), static_cast<type_t>(8));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 1), static_cast<type_t>(7));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 2), static_cast<type_t>(6));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 3), static_cast<type_t>(5));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 4), static_cast<type_t>(4));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 5), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 6), static_cast<type_t>(2));
	EXPECT_EQ(Dod::DataUtils::get(sortedBuffer, 7), static_cast<type_t>(1));

}

TEST(DataUtils, CreateSortedImBufferFailed)
{

	using type_t = int32_t;
	std::array<type_t, 8> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	Dod::ImBuffer<int32_t> buffer;
	Dod::DataUtils::initFromArray(buffer, values);

	std::array<type_t, 4> indices{ {7, 6, 5, 4} };
	Dod::ImBuffer<int32_t> indicesBuffer;
	Dod::DataUtils::initFromArray(indicesBuffer, indices);

	Dod::SortedImBuffer<int32_t> sortedBuffer{ Dod::DataUtils::createSortedImBuffer(buffer, indicesBuffer) };

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(sortedBuffer), 0);

}

TEST(DBBufferUtils, GetLen)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
	std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;
	std::memcpy(memory.data(), values.data(), memory.size());

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);
	Dod::DataUtils::populate(buffer, 10, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	Dod::DataUtils::populate(buffer, 20, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	Dod::DataUtils::populate(buffer, 30, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
	Dod::DataUtils::populate(buffer, 40, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
	Dod::DataUtils::populate(buffer, 50, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 5);
	Dod::DataUtils::populate(buffer, 60, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 6);
	Dod::DataUtils::populate(buffer, 70, true);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 7);

}

TEST(DataUtils, GetLen)
{

	using type_t = int32_t;

	{
		constexpr size_t totalElements{ 8 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		CommonBuffer<int32_t> buffer;

		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 0 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		CommonBuffer<int32_t> buffer;

		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 1 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		CommonBuffer<int32_t> buffer;

		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 8 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		Dod::MutBuffer<int32_t> buffer;

		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 0 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		Dod::MutBuffer<int32_t> buffer;

		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 1 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		Dod::MutBuffer<int32_t> buffer;

		Dod::DataUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), totalElements);
	}
}

TEST(DBBufferUtils, PopulationPartialMemory)
{

	std::array<Dod::MemTypes::data_t, 13> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	Dod::DataUtils::initFromMemory(buffer, memSpan);

	{
		const int32_t value{ 1 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
	}
	{
		const int32_t value{ 2 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}
	{
		const int32_t value{ 3 };
		Dod::DataUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 0), 3);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
	}

}

TEST(DBBufferUtils, PopulationConditional)
{

	using type_t = int32_t;

	constexpr size_t totalValues{ 1'000 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;
	Dod::DataUtils::initFromMemory(buffer, memSpan);

	for (type_t value{ 1 }; value < totalValues + 1; ++value)
	{
		const auto initialFilledEls{ Dod::DataUtils::getNumFilledElements(buffer) };

		const auto bNeedAdd{ value % 2 == 0 };
		Dod::DataUtils::populate(buffer, value, bNeedAdd);

		const auto currentFilledEls{ Dod::DataUtils::getNumFilledElements(buffer) };

		if (bNeedAdd)
			EXPECT_EQ(currentFilledEls - initialFilledEls, 1);
		else
			EXPECT_EQ(currentFilledEls - initialFilledEls, 0);

	}

	int32_t totalElements{ 0 };
	for (int32_t id{ 0 }; const auto value : std::ranges::views::iota(1, static_cast<int32_t>(totalValues) + 1)
		| std::ranges::views::filter([](auto val) { return val % 2 == 0; }))
	{
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + id + 1), value);
		++id;
		++totalElements;
	}
	EXPECT_EQ(totalElements, Dod::DataUtils::getNumFilledElements(buffer));

}

TEST(DBBufferUtils, RemoveElements)
{
	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalElementsToRemove{ totalElements };
	constexpr size_t totalBytesForEls{ totalElements * sizeof(type_t) };
	constexpr size_t totalBytesForRemove{ totalElementsToRemove * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytesForEls> memory;
	MemorySpan memSpan(memory.data(), memory.data() + totalBytesForEls);

	std::array<type_t, totalElements> values{ {0, 1, 2, 3, 4, 5, 6, 7} };

	Dod::DBBuffer<int32_t> buffer;
	Dod::DataUtils::initFromMemory(buffer, memSpan);

	{

		buffer.numOfFilledEls = totalElements - 1;
		std::memcpy(memory.data(), values.data(), totalBytesForEls);

		std::array<type_t, 4> indicesToRemove{ {0, 1, 2, 3} };
		Dod::ImBuffer<int32_t> bufferIndicesToRemove;
		
		bufferIndicesToRemove.dataBegin = reinterpret_cast<const int32_t*>(indicesToRemove.data());
		bufferIndicesToRemove.dataEnd = reinterpret_cast<const int32_t*>(indicesToRemove.data() + indicesToRemove.size());

		Dod::DataUtils::remove(buffer, bufferIndicesToRemove);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 7);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), 6);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), 5);

	}
	{

		buffer.numOfFilledEls = totalElements - 1;
		std::memcpy(memory.data(), values.data(), totalBytesForEls);

		std::array<type_t, 7> indicesToRemove{ {0, 1, 2, 3, 4, 5, 6} };
		Dod::ImBuffer<int32_t> bufferIndicesToRemove;
		bufferIndicesToRemove.dataBegin = reinterpret_cast<const int32_t*>(indicesToRemove.data());
		bufferIndicesToRemove.dataEnd = reinterpret_cast<const int32_t*>(indicesToRemove.data() + indicesToRemove.size());

		Dod::DataUtils::remove(buffer, bufferIndicesToRemove);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);

	}

}

TEST(DBBufferUtils, Flush)
{

	using type_t = int32_t;

	constexpr size_t totalValues{ 128 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
	Dod::DBBuffer<int32_t> bufferDst;
	Dod::DataUtils::initFromArray(bufferDst, memoryDst);

	for (int32_t elId{}; elId < 32; ++elId)
		Dod::DataUtils::populate(bufferDst, elId + 1, true);

	Dod::DataUtils::flush(bufferDst);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 0);

	Dod::DataUtils::flush(bufferDst);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 0);

}

TEST(DBBufferUtils, GetCapacity)
{
	using type_t = int32_t;

	{
		constexpr size_t totalValues{ 128 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::DataUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::DataUtils::initFromArray(bufferSrc, memorySrc);

		EXPECT_EQ(Dod::DataUtils::getCapacity(bufferSrc), totalValues);
	}
	{
		constexpr size_t totalValues{ 64 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::DataUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::DataUtils::initFromArray(bufferSrc, memorySrc);

		EXPECT_EQ(Dod::DataUtils::getCapacity(bufferSrc), totalValues);
	}
	{
		constexpr size_t totalValues{ 64 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::DataUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::DataUtils::initFromArray(bufferSrc, memorySrc);
		bufferSrc.numOfFilledEls = 32;

		EXPECT_EQ(Dod::DataUtils::getCapacity(bufferSrc), totalValues);
	}
	{
		constexpr size_t totalValues{ 0 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::DataUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::DataUtils::initFromArray(bufferSrc, memorySrc);
		bufferSrc.numOfFilledEls = 32;

		EXPECT_EQ(Dod::DataUtils::getCapacity(bufferSrc), totalValues);
	}

}

TEST(DBBufferUtils, Append)
{

	using type_t = int32_t;

	constexpr size_t totalValues{ 128 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
	Dod::DBBuffer<int32_t> bufferDst;
	Dod::DataUtils::initFromArray(bufferDst, memoryDst);

	std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
	Dod::DBBuffer<int32_t> bufferSrc;
	Dod::DataUtils::initFromArray(bufferSrc, memorySrc);

	for (int32_t elId{}; elId < 32; ++elId)
		Dod::DataUtils::populate(bufferSrc, elId + 1, true);

	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 32);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{32}; elId < 64; ++elId)
		Dod::DataUtils::populate(bufferSrc, elId + 1, true);

	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 64 }; elId < 192; ++elId)
		Dod::DataUtils::populate(bufferSrc, elId + 1, true);
	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 192 }; elId < 256; ++elId)
		Dod::DataUtils::populate(bufferSrc, elId + 1, true);
	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), elId + 1);

}

TEST(DBBufferUtils, AppendComplex)
{

	struct Type
	{
		std::vector<int32_t> data;
		[[nodiscard]] auto operator<=>(const Type&) const = default;
	};

	using type_t = Type;

	constexpr size_t totalValues{ 128 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
	Dod::DBBuffer<type_t> bufferDst;
	Dod::DataUtils::initFromArray(bufferDst, memoryDst);

	std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
	Dod::DBBuffer<type_t> bufferSrc;
	Dod::DataUtils::initFromArray(bufferSrc, memorySrc);

	const auto initData = [](int32_t elId) {
		return std::vector<int32_t>(static_cast<size_t>(elId) + 1, 10);
	};

	for (int32_t elId{}; elId < 32; ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
	}

	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 32);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 32 }; elId < 64; ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
	}

	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 64 }; elId < 192; ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
	}

	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 192 }; elId < 256; ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
	}

	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	Dod::DataUtils::append(bufferDst, Dod::DataUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::DataUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = initData(elId);
		Dod::DataUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::DataUtils::get(bufferDst, elId), element);
	}

}
