#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/Buffers.h>
#include <dod/BufferUtils.h>
#include <dod/Algorithms.h>


#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)
#include <ranges>

struct MemorySpan
{
	Dod::MemTypes::dataPoint_t dataBegin{};
	Dod::MemTypes::dataPoint_t dataEnd{};
};

template <typename T>
struct CommonBuffer
{
	using type_t = typename T;
	const T* dataBegin{ nullptr };
	const T* dataEnd{ nullptr };
};

TEST(BufferUtils, Initialization)
{

	constexpr auto dataSize{ 32 };
	std::array<Dod::MemTypes::data_t, dataSize> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	{
		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 8);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 8 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 24 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	} 
	{
		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 8);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		MemorySpan memSpan2(memory.data() + 2, memory.data() + 8);

		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 6 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan2);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
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
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 8 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 24 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		MemorySpan memSpan2(memory.data() + 2, memory.data() + 8);

		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 6 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan2);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataBegin), memSpan2.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(buffer.dataEnd), memSpan2.dataBegin + endIndex);
	}

}

TEST(DBBufferUtils, InitializationFailed)
{

	std::array<Dod::MemTypes::data_t, 32> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 40 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 35 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 40 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 10 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 5 };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

}

TEST(BufferUtils, InitFromArray)
{

	{
		std::array<Dod::MemTypes::data_t, 1024> memory;
		Dod::DBBuffer<Dod::MemTypes::data_t> buffer;
		Dod::BufferUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, memory.data());
		EXPECT_EQ(buffer.dataEnd, memory.data() + memory.size());
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	}
	{
		std::array<Dod::MemTypes::data_t, 1024> memory;
		Dod::ImBuffer<Dod::MemTypes::data_t> buffer;
		Dod::BufferUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, memory.data());
		EXPECT_EQ(buffer.dataEnd, memory.data() + memory.size());
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), memory.size());
	}
	{
		std::array<Dod::MemTypes::data_t, 1024> memory;
		Dod::MutBuffer<Dod::MemTypes::data_t> buffer;
		Dod::BufferUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, memory.data());
		EXPECT_EQ(buffer.dataEnd, memory.data() + memory.size());
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), memory.size());
	}

	{
		std::array<int32_t, 128> memory;
		Dod::DBBuffer<Dod::MemTypes::data_t> buffer;
		Dod::BufferUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data()));
		EXPECT_EQ(buffer.dataEnd, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data() + memory.size()));
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	}
	{
		std::array<int32_t, 128> memory;
		Dod::ImBuffer<Dod::MemTypes::data_t> buffer;
		Dod::BufferUtils::initFromArray(buffer, memory);

		EXPECT_EQ(buffer.dataBegin, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data()));
		EXPECT_EQ(buffer.dataEnd, reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(memory.data() + memory.size()));
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), memory.size() * sizeof(int32_t));
	}

}

TEST(DBBufferUtils, Population)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	{
		const int32_t value{ 1 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
	}
	{
		const int32_t value{ 2 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
	}
	{
		const int32_t value{ 3 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 3), 3);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		const int32_t value{ 4 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 0), 4);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 3), 3);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}

}

TEST(DBBufferUtils, PopulationComplex)
{

	std::array<Dod::MemTypes::data_t, 128> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	using type_t = std::vector<int32_t>;

	Dod::DBBuffer<type_t> buffer;
	Dod::BufferUtils::initFromMemory(buffer, memSpan);

	const type_t v1{ 1, 2, 3, 4, 5 };
	const type_t v2{ 5, 4, 3, 2, 1 };
	const type_t v3{ 6, 7, 8, 9, 4 };
	const type_t v4{ 4, 9, 7, 8, 6 };
	const type_t v5{ 2, 1, 4, 5, 3 };

	{
		Dod::BufferUtils::populate(buffer, v1, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::BufferUtils::populate(buffer, v2, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::BufferUtils::populate(buffer, v3, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), v3);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::BufferUtils::populate(buffer, v4, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), v3);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::BufferUtils::populate(buffer, v5, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), v1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), v2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), v3);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
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

	Dod::BufferUtils::initFromArray(buffer, memory);

	{
		Dod::BufferUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1).value, 0);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2).value, 0);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::BufferUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2).value, 0);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::BufferUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::BufferUtils::constructBack(buffer, false);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3).value, 0);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::BufferUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3).value, 42);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::BufferUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2).value, 42);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3).value, 42);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
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

	Dod::BufferUtils::initFromArray(buffer, memory);

	const Type typeReal1(123, { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 });
	const Type typeReal2(456, { 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45 });
	const Type typeReal3(789, { 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65 });
	const Type typeReal4(753, { 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85 });
	const Type typeEmpty(0, {});

	{
		Dod::BufferUtils::constructBack(buffer, typeReal1, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
	}
	{
		Dod::BufferUtils::constructBack(buffer, typeReal2, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
	}
	{
		Dod::BufferUtils::constructBack(buffer, typeReal3, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::BufferUtils::constructBack(buffer, false);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeEmpty);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	}
	{
		Dod::BufferUtils::constructBack(buffer, typeReal4, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::BufferUtils::constructBack(buffer, typeReal4, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
	}
	{
		Dod::BufferUtils::constructBack(buffer, true);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), typeReal1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), typeReal2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), typeReal3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), typeReal4);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
	}
}

TEST(BufferUtils, DBBufferBound)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	using type_t = int32_t;
	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	const auto boundSpan{ Dod::BufferUtils::getBufferBounds(buffer) };
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(boundSpan.dataBegin), memSpan.dataBegin + sizeof(type_t));
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(boundSpan.dataEnd), memSpan.dataEnd);

}

TEST(BufferUtils, BufferBound)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	using type_t = int32_t;
	CommonBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	const auto boundSpan{ Dod::BufferUtils::getBufferBounds(buffer) };
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(boundSpan.dataBegin), memSpan.dataBegin);
	EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(boundSpan.dataEnd), memSpan.dataEnd);

}

TEST(BufferUtils, InitializationFromDBBuffer)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());
	Dod::DBBuffer<int32_t> srcBuffer;

	Dod::BufferUtils::initFromMemory(srcBuffer, memSpan, beginIndex, endIndex);

	for (const auto value : std::to_array<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8 }))
		Dod::BufferUtils::populate(srcBuffer, value, true);

	{
		CommonBuffer<int32_t> imBuffer;

		Dod::BufferUtils::initFromBuffer(imBuffer, srcBuffer);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(imBuffer), Dod::BufferUtils::getNumFilledElements(srcBuffer));
	}
	{
		CommonBuffer<int32_t> imBuffer;

		Dod::BufferUtils::initFromBuffer(imBuffer, srcBuffer, 0, Dod::BufferUtils::getNumFilledElements(srcBuffer));
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(imBuffer), Dod::BufferUtils::getNumFilledElements(srcBuffer));
	}
	{
		CommonBuffer<int32_t> imBuffer;

		Dod::BufferUtils::initFromBuffer(imBuffer, srcBuffer, 0, 4);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataBegin + 5);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(imBuffer), 4);
	}
	{
		CommonBuffer<int32_t> imBuffer;

		Dod::BufferUtils::initFromBuffer(imBuffer, srcBuffer, 2, 5);
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 3);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataBegin + 6);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(imBuffer), 3);
	}

}

TEST(BufferUtils, CreateImFromBuffer)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());
	Dod::DBBuffer<int32_t> srcBuffer;
	Dod::BufferUtils::initFromMemory(srcBuffer, memSpan, beginIndex, endIndex);

	for (const auto value : std::to_array<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8 }))
		Dod::BufferUtils::populate(srcBuffer, value, true);

	const auto imBuffer{ Dod::BufferUtils::createImFromBuffer(srcBuffer) };
	{
		EXPECT_EQ(imBuffer.dataBegin, srcBuffer.dataBegin + 1);
		EXPECT_EQ(imBuffer.dataEnd, srcBuffer.dataEnd);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(imBuffer), Dod::BufferUtils::getNumFilledElements(srcBuffer));
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

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), static_cast<type_t>(2));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), static_cast<type_t>(3));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), static_cast<type_t>(4));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), static_cast<type_t>(5));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 4), static_cast<type_t>(6));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 5), static_cast<type_t>(7));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 6), static_cast<type_t>(8));

	Dod::BufferUtils::get(buffer, 6) = 42;
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 6), static_cast<type_t>(42));

}

TEST(BufferUtils, GetValue)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
	std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;
	std::memcpy(memory.data(), values.data(), memory.size());

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	CommonBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), static_cast<type_t>(1));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), static_cast<type_t>(2));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), static_cast<type_t>(3));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), static_cast<type_t>(4));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 4), static_cast<type_t>(5));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 5), static_cast<type_t>(6));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 6), static_cast<type_t>(7));
	EXPECT_EQ(Dod::BufferUtils::get(buffer, 7), static_cast<type_t>(8));
			
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

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	Dod::BufferUtils::populate(buffer, 10, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
	Dod::BufferUtils::populate(buffer, 20, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
	Dod::BufferUtils::populate(buffer, 30, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
	Dod::BufferUtils::populate(buffer, 40, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
	Dod::BufferUtils::populate(buffer, 50, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 5);
	Dod::BufferUtils::populate(buffer, 60, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 6);
	Dod::BufferUtils::populate(buffer, 70, true);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 7);

}

TEST(BufferUtils, GetLen)
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

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 0 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 1 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		CommonBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 8 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		Dod::MutBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 0 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		Dod::MutBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), totalElements);
	}

	{
		constexpr size_t totalElements{ 1 };
		constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
		std::array<type_t, totalElements> values{};

		std::array<Dod::MemTypes::data_t, totalBytes> memory;
		std::memcpy(memory.data(), values.data(), memory.size());

		MemorySpan memSpan(memory.data(), memory.data() + memory.size());

		Dod::MutBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
		Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), totalElements);
	}
}

TEST(DBBufferUtils, PopulationPartialMemory)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 13 };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	{
		const int32_t value{ 1 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
	}
	{
		const int32_t value{ 2 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
	}
	{
		const int32_t value{ 3 };
		Dod::BufferUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 0), 3);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
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
	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalMemory };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	for (type_t value{ 1 }; value < totalValues + 1; ++value)
	{
		const auto initialFilledEls{ Dod::BufferUtils::getNumFilledElements(buffer) };

		const auto bNeedAdd{ value % 2 == 0 };
		Dod::BufferUtils::populate(buffer, value, bNeedAdd);

		const auto currentFilledEls{ Dod::BufferUtils::getNumFilledElements(buffer) };

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
	EXPECT_EQ(totalElements, Dod::BufferUtils::getNumFilledElements(buffer));

}

TEST(DBBufferUtils, RemoveElements)
{
	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalElementsToRemove{ totalElements };
	constexpr size_t totalBytesForEls{ totalElements * sizeof(type_t) };
	constexpr size_t totalBytesForRemove{ totalElementsToRemove * sizeof(type_t) };
	constexpr size_t totalBytes{ (totalElements + totalElementsToRemove) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;
	MemorySpan memSpan(memory.data(), memory.data() + totalBytesForEls);

	std::array<type_t, totalElements> values{ {0, 1, 2, 3, 4, 5, 6, 7} };

	Dod::DBBuffer<int32_t> buffer;
	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalBytesForEls };
	Dod::BufferUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	{

		buffer.numOfFilledEls = totalElements - 1;
		std::memcpy(memory.data(), values.data(), totalBytesForEls);

		std::array<type_t, 4> indicesToRemove{ {0, 1, 2, 3} };
		Dod::ImBuffer<int32_t> bufferIndicesToRemove;
		
		bufferIndicesToRemove.dataBegin = reinterpret_cast<const int32_t*>(indicesToRemove.data());
		bufferIndicesToRemove.dataEnd = reinterpret_cast<const int32_t*>(indicesToRemove.data() + indicesToRemove.size());

		Dod::BufferUtils::remove(buffer, bufferIndicesToRemove);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 7);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 6);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), 5);

	}
	{

		buffer.numOfFilledEls = totalElements - 1;
		std::memcpy(memory.data(), values.data(), totalBytesForEls);

		std::array<type_t, 7> indicesToRemove{ {0, 1, 2, 3, 4, 5, 6} };
		Dod::ImBuffer<int32_t> bufferIndicesToRemove;
		bufferIndicesToRemove.dataBegin = reinterpret_cast<const int32_t*>(indicesToRemove.data());
		bufferIndicesToRemove.dataEnd = reinterpret_cast<const int32_t*>(indicesToRemove.data() + indicesToRemove.size());

		Dod::BufferUtils::remove(buffer, bufferIndicesToRemove);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);

	}

}

TEST(DBBufferUtils, Flush)
{

	using type_t = int32_t;

	constexpr size_t totalValues{ 128 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
	Dod::DBBuffer<int32_t> bufferDst;
	Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

	for (int32_t elId{}; elId < 32; ++elId)
		Dod::BufferUtils::populate(bufferDst, elId + 1, true);

	Dod::BufferUtils::flush(bufferDst);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 0);

	Dod::BufferUtils::flush(bufferDst);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 0);

}

TEST(DBBufferUtils, GetCapacity)
{
	using type_t = int32_t;

	{
		constexpr size_t totalValues{ 128 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::BufferUtils::initFromArray(bufferSrc, memorySrc);

		EXPECT_EQ(Dod::BufferUtils::getCapacity(bufferSrc), totalValues);
	}
	{
		constexpr size_t totalValues{ 64 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::BufferUtils::initFromArray(bufferSrc, memorySrc);

		EXPECT_EQ(Dod::BufferUtils::getCapacity(bufferSrc), totalValues);
	}
	{
		constexpr size_t totalValues{ 64 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::BufferUtils::initFromArray(bufferSrc, memorySrc);
		bufferSrc.numOfFilledEls = 32;

		EXPECT_EQ(Dod::BufferUtils::getCapacity(bufferSrc), totalValues);
	}
	{
		constexpr size_t totalValues{ 0 };
		constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

		std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
		Dod::DBBuffer<int32_t> bufferDst;
		Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

		std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
		Dod::DBBuffer<int32_t> bufferSrc;
		Dod::BufferUtils::initFromArray(bufferSrc, memorySrc);
		bufferSrc.numOfFilledEls = 32;

		EXPECT_EQ(Dod::BufferUtils::getCapacity(bufferSrc), totalValues);
	}

}

TEST(DBBufferUtils, Append)
{

	using type_t = int32_t;

	constexpr size_t totalValues{ 128 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memoryDst;
	Dod::DBBuffer<int32_t> bufferDst;
	Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

	std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
	Dod::DBBuffer<int32_t> bufferSrc;
	Dod::BufferUtils::initFromArray(bufferSrc, memorySrc);

	for (int32_t elId{}; elId < 32; ++elId)
		Dod::BufferUtils::populate(bufferSrc, elId + 1, true);

	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 32);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{32}; elId < 64; ++elId)
		Dod::BufferUtils::populate(bufferSrc, elId + 1, true);

	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 64 }; elId < 192; ++elId)
		Dod::BufferUtils::populate(bufferSrc, elId + 1, true);
	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 192 }; elId < 256; ++elId)
		Dod::BufferUtils::populate(bufferSrc, elId + 1, true);
	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), elId + 1);

	bufferSrc.numOfFilledEls = 0;
	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), elId + 1);

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
	Dod::BufferUtils::initFromArray(bufferDst, memoryDst);

	std::array<Dod::MemTypes::data_t, totalMemory> memorySrc;
	Dod::DBBuffer<type_t> bufferSrc;
	Dod::BufferUtils::initFromArray(bufferSrc, memorySrc);

	for (int32_t elId{}; elId < 32; ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
	}

	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 32);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 32 }; elId < 64; ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
	}

	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 64);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 64 }; elId < 192; ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
	}

	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	for (int32_t elId{ 192 }; elId < 256; ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
	}

	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), element);
	}

	bufferSrc.numOfFilledEls = 0;
	Dod::BufferUtils::append(bufferDst, Dod::BufferUtils::createImFromBuffer(bufferSrc));
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(bufferDst), 128);
	for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(bufferDst); ++elId)
	{
		Type element;
		element.data = std::vector<int32_t>(elId + 1, 10);
		Dod::BufferUtils::populate(bufferSrc, element, true);
		EXPECT_EQ(Dod::BufferUtils::get(bufferDst, elId), element);
	}

}
