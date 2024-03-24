#include <gtest/gtest.h>

#include <dod/MemTypes.h>
#include <dod/TableUtils.h>
#include <dod/Tables.h>
#include <dod/MemPool.h>

#include "utils/CommonStructures.h"

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#include <span>
#include <cassert>
#include <cstring>
#pragma warning(pop)

static constexpr Dod::MemTypes::alignment_t alignment{ 64 };

static constexpr size_t bytesForInt7{ 64 };
static constexpr size_t bytesForInt20{ 128 };
static constexpr size_t bytesForIntFloat3{ 128 };
static constexpr size_t bytesForIntFloat7{ 128 };
static constexpr size_t bytesForIntDouble2{ 128 };
static constexpr size_t bytesForIntDouble3{ 128 };
static constexpr size_t bytesForIntDouble4{ 128 };
static constexpr size_t bytesForIntDouble16{ 192 };
static constexpr size_t bytesForDoubleInt2{ 128 };
static constexpr size_t bytesForIntComplex2{ 196 };
static constexpr size_t bytesForIntComplex3{ 196 };
static constexpr size_t bytesForIntInt{ 256 };
static constexpr size_t bytesForIntComplex4{ 256 };
static constexpr size_t bytesForIntComplexComplex3{ 320 };

template <typename ... Types>
static void checkMemoryAlignment(auto&& memory)
{
	using types_t = std::tuple<Types...>;
	constexpr auto maxAlignment{ RisingCore::Helpers::findLargestAlignment<types_t>() };
	const auto address{ reinterpret_cast<std::uintptr_t>(memory.data()) };
	ASSERT_EQ(address % maxAlignment, 0);
}

template <size_t memorySize, typename ... Types>
class InitDBTableFromMemoryTest : public ::testing::Test {

protected:
	void run(int32_t expectCapacityEls, Dod::MemTypes::capacity_t expectedBytesRequires)
	{
		this->runImpl(this->table, expectCapacityEls, expectedBytesRequires);
//		this->runImpl(this->tableMut, expectCapacityEls, expectedBytesRequires);
//		this->runImpl(this->tableIm, expectCapacityEls, expectedBytesRequires);
	}

private:
	void runImpl(auto& table, int32_t expectCapacityEls, Dod::MemTypes::capacity_t expectedBytesRequires)
	{

		checkMemoryAlignment<Types...>(this->memory);

		EXPECT_EQ(Dod::DataUtils::computeCapacityInBytes<RisingCore::Helpers::gatherTypes<Types...>>(expectCapacityEls), expectedBytesRequires);

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(table, expectCapacityEls, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table), 0);

		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(this->table.dataBegin), memSpan.dataBegin);
		ASSERT_EQ(Dod::DataUtils::getCapacity(this->table), expectCapacityEls);

	}

public:

	alignas(alignment) std::array<Dod::MemTypes::data_t, memorySize + alignment> memory{};
	Dod::DTable<Types...> table;
	Dod::MutTable<Types...> tableMut;
	Dod::ImTable<Types...> tableIm;

};

using InitDBTableFromMemory32BytesInt = InitDBTableFromMemoryTest<bytesForInt7, int>;
TEST_F(InitDBTableFromMemory32BytesInt, InitFromMemory)
{
	this->run(0, 0);
	this->run(1, 64);
	this->run(7, 64);
}

using InitDBTableFromMemory128BytesInt = InitDBTableFromMemoryTest<bytesForInt20, int>;
TEST_F(InitDBTableFromMemory128BytesInt, InitFromMemory)
{
	this->run(0, 0);
	this->run(1, 64);
	this->run(7, 64);
	this->run(16, 64);
	this->run(18, 128);
	this->run(20, 128);
}

using InitDBTableFromMemory32BytesIntFloat = InitDBTableFromMemoryTest<bytesForIntFloat3, int, float>;
TEST_F(InitDBTableFromMemory32BytesIntFloat, InitFromMemory)
{

	this->run(1, 128);
	this->run(3, 128);

}

using InitDBTableFromMemory32BytesIntDouble = InitDBTableFromMemoryTest<bytesForIntDouble2, int, double>;
TEST_F(InitDBTableFromMemory32BytesIntDouble, InitFromMemory)
{
	this->run(0, 0);
	this->run(1, 128);
	this->run(2, 128);
}

using InitDBTableFromMemory3IntDouble = InitDBTableFromMemoryTest<bytesForIntDouble3, int, double>;
TEST_F(InitDBTableFromMemory3IntDouble, InitFromMemory)
{
	this->run(0, 0);
	this->run(2, 128);
	this->run(3, 128);
}

template <size_t memorySize, Dod::MemTypes::alignment_t memoryAlignmentOffset, typename ... Types>
class TableTest : public ::testing::Test {
	using types_t = std::tuple<Types...>;
protected:
	void init(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data() + memoryAlignmentOffset, this->memory.data() + memoryAlignmentOffset + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, expectCapacityEls, memSpan);
		ASSERT_EQ(Dod::DataUtils::getCapacity(this->table), expectCapacityEls);
		ASSERT_GE(this->table.dataBegin, this->memory.data());

		const auto address{ reinterpret_cast<std::uintptr_t>(this->table.dataBegin) };
		EXPECT_EQ(address % alignment, 0);

	}

	void pushBack(int32_t expectedNumOfFilledEls, auto&& ... values)
	{
		Dod::DataUtils::pushBack(this->table, true, std::forward<decltype(values)>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	void pushBackReject(int32_t expectedNumOfFilledEls, auto&& ... values)
	{
		Dod::DataUtils::pushBack(this->table, false, std::forward<decltype(values)>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	void pushBackDummy(int32_t expectedNumOfFilledEls, auto&& ... values)
	{
		Dod::DataUtils::pushBack(this->table, false, std::forward<decltype(values)>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	void populate(int32_t expectedNumOfFilledEls, int32_t numOfElementsToFill, auto&& ... fillers)
	{
		Dod::DataUtils::populate(this->table, numOfElementsToFill, std::forward<decltype(fillers)>(fillers)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	template <typename T>
	void checkInternal(std::vector<T> expectedValues, int32_t offset)
	{
		const auto populatedPtr{ reinterpret_cast<T*>(table.dataBegin + static_cast<size_t>(offset)) };

		for (int32_t id{}; const auto expected : expectedValues)
		{
			const auto point{ populatedPtr + (id++) };
			EXPECT_EQ(*point, expected);

			const auto address{ reinterpret_cast<std::uintptr_t>(point) };
			constexpr auto expectedAlignment{ alignof(T) };
			EXPECT_EQ(address % expectedAlignment, 0);
		}
	}

	template <int32_t columnId, typename T>
	void check(std::vector<T>&& expectedValues, int32_t offset)
	{

		this->checkInternal<T>(expectedValues, offset);

		const auto impl = [&]<typename TBuffer>(auto&& table) {

			const auto checker = [&](auto&& data) {
				ASSERT_EQ(Dod::DataUtils::getNumFilledElements(data), expectedValues.size());

				constexpr auto expectedAlignment{ alignof(T) };
				for (int32_t id{}; auto && expected : expectedValues)
				{
					auto&& value{ Dod::DataUtils::get(data, id++) };
					EXPECT_EQ(value, expected);
					EXPECT_EQ(reinterpret_cast<std::uintptr_t>(&value) % expectedAlignment, 0);
				}
			};

			if constexpr (Dod::CommonData::CMonoData<std::decay_t<decltype(table)>>)
			{
				checker(table);
			}
			else
			{
				TBuffer col{ Dod::DataUtils::get<columnId>(table) };
				checker(col);
			}

		};

		impl.template operator()<Dod::MutTable<T>>(this->table);
		impl.template operator()<Dod::MutTable<T>>(Dod::MutTable(this->table));
		impl.template operator()<Dod::ImTable<T>>(Dod::ImTable(this->table));

	}

	template <typename ... T>
	void checkTable(std::tuple<std::vector<T>...>&& expectedValues) requires requires(T ... t) {
		{std::tuple_size_v<std::tuple<T...>> > 1 };
	}
	{

		const auto impl = [&](auto&& data) {
			const auto columns{ Dod::DataUtils::get(data) };
			constexpr auto receivedNumOfColumns{ std::tuple_size_v<std::decay_t<decltype(columns)>> };
			constexpr auto expectedNumOfColumns{ std::tuple_size_v<std::decay_t<decltype(expectedValues)>> };
			ASSERT_EQ(receivedNumOfColumns, expectedNumOfColumns);

			RisingCore::Helpers::constexprLoop<receivedNumOfColumns>([&]<size_t columnId>() {
				auto&& col{ std::get<columnId>(columns) };
				auto&& expectedColumn{ std::get<columnId>(expectedValues) };
				ASSERT_EQ(Dod::DataUtils::getNumFilledElements(col), expectedColumn.size());

				using columnBufferType_t = std::decay_t<decltype(col)>;
				using columnType_t = columnBufferType_t::types_t;

				constexpr auto expectedAlignment{ alignof(columnType_t) };
				for (int32_t id{}; auto && expected : expectedColumn)
				{
					auto&& value{ Dod::DataUtils::get(col, id++) };
					EXPECT_EQ(value, expected);
					EXPECT_EQ(reinterpret_cast<std::uintptr_t>(&value) % expectedAlignment, 0);
				}
			});
		};

		impl(this->table);
		impl(Dod::MutTable(this->table));
		impl(Dod::ImTable(this->table));


	}

	template <typename... Ts>
	struct AllSameType : std::conjunction<std::is_same<Ts, typename std::tuple_element<0, std::tuple<Ts...>>::type>...> {};

	template <typename T, typename ... TOther>
	[[nodiscard]] auto genCheckVector(T&& first, TOther&& ... other) -> std::vector<T> {
		static_assert(AllSameType<T, TOther...>{});
		std::vector<T> out;
		out.emplace_back(std::forward<T>(first));

		(out.emplace_back(std::forward<TOther>(other)), ...);
		return out;
	};

	template <typename ... T>
	[[nodiscard]] auto genCheckTuple(std::vector<T>&& ... values) -> std::tuple<std::vector<T>...> {
		return std::make_tuple(std::forward<std::vector<T>>(values)...);
	};

	void remove(int32_t expectedEls, std::vector<int32_t> indices)
	{
		Dod::MemPool memory;
		memory.allocate(1024 + alignment);
		const auto memorySpan{ Dod::MemUtils::acquire(memory, 0, 1024, alignment) };
		Dod::ImTable<int32_t> indicesToRemove;
		std::memcpy(memorySpan.dataBegin, indices.data(), indices.size() * sizeof(int32_t));
		Dod::DataUtils::initFromMemory(indicesToRemove, static_cast<int32_t>(indices.size()), static_cast<int32_t>(indices.size()), memorySpan);
		Dod::DataUtils::remove(this->table, indicesToRemove);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), expectedEls);
	}

	alignas(alignment) std::array<Dod::MemTypes::data_t, memorySize + memoryAlignmentOffset> memory{};
	Dod::DTable<Types...> table;

};

using PushBackInt20 = TableTest<bytesForInt20, 0, int>;
TEST_F(PushBackInt20, PushBack)
{

	this->init(20);

	this->pushBack(1, 1);
	this->check<0, int>({ 1 }, 0);

	this->pushBack(2, 2);
	this->check<0, int>({ 1, 2 }, 0);

	this->pushBackReject(2, 3);
	this->check<0, int>({ 1, 2 }, 0);

	this->pushBack(3, 4);
	this->check<0, int>({ 1, 2, 4 }, 0);

}

using PushBackIntFloat = TableTest<bytesForIntFloat3, 0, int, float>;
TEST_F(PushBackIntFloat, PushBack)
{

	this->init(3);

	this->pushBack(1, 1, 2.f);
	this->check<0, int>({ 1 }, 0);
	this->check<1, float>({ 2.f }, 64);

	this->pushBack(2, 3, 4.f);
	this->check<0, int>({ 1, 3 }, 0);
	this->check<1, float>({ 2.f, 4.f }, 64);

	this->pushBackReject(2, 5, 6.f);
	this->check<0, int>({ 1, 3 }, 0);
	this->check<1, float>({ 2.f, 4.f }, 64);

	this->pushBack(3, 7, 8.f);
	this->check<0, int>({ 1, 3, 7 }, 0);
	this->check<1, float>({ 2.f, 4.f, 8 }, 64);

	this->pushBack(3, 9, 10.f);

}

using PushBackIntDoubleAlignment = TableTest<bytesForIntDouble3, 0, int, double>;
TEST_F(PushBackIntDoubleAlignment, PushBack)
{

	this->init(3);

	this->pushBack(1, 1, 2.);
	this->check<0, int>({ 1 }, 0);
	this->check<1, double>({ 2. }, 64);

	this->pushBack(2, 3, 4.);
	this->check<0, int>({ 1, 3 }, 0);
	this->check<1, double>({ 2., 4. }, 64);

	this->pushBack(3, 5, 6.);
	this->check<0, int>({ 1, 3, 5 }, 0);
	this->check<1, double>({ 2., 4., 6. }, 64);

	this->pushBack(3, 7, 8.);

}

using PushBackDoubleInt = TableTest<bytesForDoubleInt2, 0, double, int>;
TEST_F(PushBackDoubleInt, PushBack)
{

	this->init(2);

	this->pushBack(1, 1., 2);
	this->check<0, double>({ 1. }, 0);
	this->check<1, int>({ 2 }, 64);

	this->pushBack(2, 3., 4);
	this->check<0, double>({ 1., 3. }, 0);
	this->check<1, int>({ 2, 4 }, 64);

	this->pushBack(2, 5., 6);

}

using PushBackIntDoubleNotAligned = TableTest<bytesForIntDouble2 + alignment, 4, int, double>;
TEST_F(PushBackIntDoubleNotAligned, PushBack)
{

	this->init(2);

	this->pushBack(1, 1, 2.);
	this->check<0, int>({ 1 }, 0);
	this->check<1, double>({ 2. }, 64);

	this->pushBack(2, 3, 4.);
	this->check<0, int>({ 1, 3 }, 0);
	this->check<1, double>({ 2., 4. }, 64);

	this->pushBack(2, 5, 6.);

}

using PushBackDoubleIntNotAligned = TableTest<bytesForDoubleInt2 + alignment, 4, double, int>;
TEST_F(PushBackDoubleIntNotAligned, PushBack)
{

	this->init(2);

	this->pushBack(1, 1., 2);
	this->check<0, double>({ 1. }, 0);
	this->check<1, int>({ 2 }, 64);

	this->pushBack(2, 3., 4);
	this->check<0, double>({ 1., 3. }, 0);
	this->check<1, int>({ 2, 4 }, 64);

	this->pushBack(2, 5., 6);

}

using GetInt20 = TableTest<bytesForInt20, 0, int>;
TEST_F(GetInt20, Get)
{

	this->init(20);

	for (int32_t id{}; id < Dod::DataUtils::getCapacity(this->table); ++id)
		Dod::DataUtils::pushBack(this->table, true, id + 1);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 20);

	for (int32_t id{}; id < Dod::DataUtils::getCapacity(this->table); ++id)
		EXPECT_EQ(Dod::DataUtils::get(this->table, id), id + 1);

}

using PopulateInt7 = TableTest<bytesForInt7, 0, int>;
TEST_F(PopulateInt7, Populate)
{

	this->init(7);
	this->populate(7, 7, [](int32_t id) {
		return id + 1;
	});
	this->checkTable<int>({ {1, 2, 3, 4, 5, 6, 7} });
	Dod::DataUtils::flush(this->table);

	this->init(7);
	this->populate(3, 3, [](int32_t id) {
		return id + 1 + 10;
	});
	this->checkTable<int>({ {11, 12, 13} });
	this->populate(5, 2, [](int32_t id) {
		return id + 1 + 20;
	});
	this->checkTable<int>({ {11, 12, 13, 21, 22} });
	this->populate(7, 2, [](int32_t id) {
		return id + 1 + 30;
		});
	this->checkTable<int>({ {11, 12, 13, 21, 22, 31, 32} });

	this->init(7);
	this->populate(0, 10, [](int32_t id) {
		return id + 1 + 40;
	});
	this->populate(2, 2, [](int32_t id) {
		return id + 1 + 50;
	});
	this->checkTable<int>({ {51, 52} });
	this->populate(2, 7, [](int32_t id) {
		return id + 1 + 60;
	});
	this->checkTable<int>({ {51, 52} });
	this->populate(7, 5, [](int32_t id) {
		return id + 1 + 70;
	});
	this->checkTable<int>({ {51, 52, 71, 72, 73, 74, 75} });
	this->populate(7, 1, [](int32_t id) {
		return id + 1 + 2 + 40;
		});
	this->checkTable<int>({ {51, 52, 71, 72, 73, 74, 75} });

}

using PopulateIntDouble16 = TableTest<bytesForIntDouble16, 0, int, double>;
TEST_F(PopulateIntDouble16, Populate)
{

	this->init(16);
	this->populate(16, 16, [](int32_t id) {
		return id + 1;
	}, [](int32_t id) {
		return static_cast<double>(id) + 20. + 1.;
	});
	this->checkTable<int, double>({ 
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
		{21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32., 33., 34., 35., 36.} 
	});
	Dod::DataUtils::flush(this->table);

	this->init(16);
	this->populate(5, 5, [](int32_t id) {
		return id + 1 + 40;
	}, [](int32_t id) {
		return static_cast<double>(id) + 60. + 1.;
	});
	this->checkTable<int, double>({
		{41, 42, 43, 44, 45},
		{61., 62., 63., 64., 65.}
	});
	this->populate(11, 6, [](int32_t id) {
		return id + 1 + 70;
	}, [](int32_t id) {
		return static_cast<double>(id) + 90. + 1.;
	});
	this->checkTable<int, double>({
		{41, 42, 43, 44, 45, 71, 72, 73, 74, 75, 76},
		{61., 62., 63., 64., 65., 91., 92., 93., 94., 95., 96.}
	});
	this->populate(16, 5, [](int32_t id) {
		return id + 1 + 100;
	}, [](int32_t id) {
		return static_cast<double>(id) + 120. + 1.;
	});
	this->checkTable<int, double>({
		{41, 42, 43, 44, 45, 71, 72, 73, 74, 75, 76, 101, 102, 103, 104, 105},
		{61., 62., 63., 64., 65., 91., 92., 93., 94., 95., 96., 121., 122., 123., 124., 125.}
	});
	Dod::DataUtils::flush(this->table);
	
	this->init(16);
	this->populate(0, 17, [](int32_t id) {
		return id + 1 + 140;
	}, [](int32_t id) {
		return static_cast<double>(id) + 160. + 1.;
	});
	this->populate(2, 2, [](int32_t id) {
		return id + 1 + 180;
	}, [](int32_t id) {
		return static_cast<double>(id) + 200. + 1.;
	});
	this->checkTable<int, double>({
		{181, 182},
		{201., 202.}
	});
	this->populate(2, 16, [](int32_t id) {
		return id + 1 + 220;
	}, [](int32_t id) {
		return static_cast<double>(id) + 240. + 1.;
	});
	this->checkTable<int, double>({
		{181, 182},
		{201., 202.}
	});
	this->populate(16, 14, [](int32_t id) {
		return id + 1 + 260;
	}, [](int32_t id) {
		return static_cast<double>(id) + 280. + 1.;
	});
	this->checkTable<int, double>({
		{181, 182, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274},
		{201., 202., 281., 282., 283., 284., 285., 286., 287., 288., 289., 290., 291., 292., 293., 294.}
	});
	this->populate(16, 1, [](int32_t id) {
		return id + 1 + 300;
	}, [](int32_t id) {
		return static_cast<double>(id) + 320. + 1.;
	});
	this->checkTable<int, double>({
		{181, 182, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274},
		{201., 202., 281., 282., 283., 284., 285., 286., 287., 288., 289., 290., 291., 292., 293., 294.}
	});

}


using PopulateIntInt1 = TableTest<bytesForIntInt, 0, int, int>;
TEST_F(PopulateIntInt1, Populate)
{

	this->init(32);
	this->populate(1, 1, [](int32_t id) {
		return id + 1;
	}, [](int32_t id) {
		return static_cast<int>(id) + 20 + 1;
	});
	this->checkTable<int, int>({
		{1},
		{21}
	});

}

using GetInt = TableTest<bytesForInt7, 0, int>;
TEST_F(GetInt, GetElements)
{

	this->init(7);

	this->pushBack(1, 1);
	this->pushBack(2, 2);
	this->pushBack(3, 3);
	this->pushBack(4, 4);
	this->pushBack(5, 5);
	this->pushBack(6, 6);
	this->pushBack(7, 7);

	EXPECT_EQ(Dod::DataUtils::get(this->table, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 1), 2);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 2), 3);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 3), 4);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 4), 5);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 5), 6);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 6), 7);

	Dod::DataUtils::get(this->table, 0) = 11;
	Dod::DataUtils::get(this->table, 1) = 12;
	Dod::DataUtils::get(this->table, 2) = 13;
	Dod::DataUtils::get(this->table, 3) = 14;
	Dod::DataUtils::get(this->table, 4) = 15;
	Dod::DataUtils::get(this->table, 5) = 16;
	Dod::DataUtils::get(this->table, 6) = 17;

	EXPECT_EQ(Dod::DataUtils::get(this->table, 0), 11);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 1), 12);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 2), 13);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 3), 14);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 4), 15);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 5), 16);
	EXPECT_EQ(Dod::DataUtils::get(this->table, 6), 17);

}

using GetIntFloat = TableTest<bytesForIntFloat3, 0, int, float>;
TEST_F(GetIntFloat, GetElements)
{

	this->init(3);

	this->pushBack(1, 1, 2.f);
	this->check<0, int>({ 1 }, 0);
	this->check<1, float>({ 2.f }, 64);

	this->pushBack(2, 3, 4.f);
	this->check<0, int>({ 1, 3 }, 0);
	this->check<1, float>({ 2.f, 4.f }, 64);

	this->pushBack(3, 5, 6.f);
	this->check<0, int>({ 1, 3, 5 }, 0);
	this->check<1, float>({ 2.f, 4.f, 6.f }, 64);

	this->pushBack(3, 7, 8.f);

}

using GetIntDouble = TableTest<bytesForIntDouble3, 0, int, double>;
TEST_F(GetIntDouble, GetElements)
{

	this->init(3);

	this->pushBack(1, 1, 2.);
	this->check<0, int>({ 1 }, 0);
	this->check<1, double>({ 2. }, 64);

	this->pushBack(2, 3, 4.);
	this->check<0, int>({ 1, 3 }, 0);
	this->check<1, double>({ 2., 4. }, 64);

	this->pushBack(3, 5, 6.);
	this->check<0, int>({ 1, 3, 5 }, 0);
	this->check<1, double>({ 2., 4., 6. }, 64);

	this->pushBack(3, 7, 8.);

}

using GetAllIntFloat = TableTest<bytesForIntFloat3, 0, int, float>;
TEST_F(GetAllIntFloat, GetAllElements)
{

	this->init(3);

	this->pushBack(1, 1, 2.f);
	this->checkTable<int, float>({ {1}, {2.f} });

	this->pushBack(2, 3, 4.f);
	this->checkTable<int, float>({ {1, 3}, {2.f, 4.f} });

	this->pushBack(3, 5, 6.f);
	this->checkTable<int, float>({ {1, 3, 5}, {2.f, 4.f, 6.f} });

	this->pushBack(3, 7, 8.f);

}

using GetAllIntDouble = TableTest<bytesForIntDouble2, 0, int, double>;
TEST_F(GetAllIntDouble, GetAllElements)
{

	this->init(2);

	this->pushBack(1, 1, 2.);
	this->checkTable<int, double>({ {1}, {2.} });

	this->pushBack(2, 3, 4.);
	this->checkTable<int, double>({ {1, 3}, {2., 4.} });

	this->pushBack(2, 5, 6.);

}

template <size_t memorySize, Dod::MemTypes::alignment_t memoryAlignmentOffset, typename ... Types>
class TableToAppend
{
public:
	TableToAppend(int32_t expectCapacityEls)
	{
		MemorySpan memSpan(this->memory.data() + memoryAlignmentOffset, this->memory.data() + memoryAlignmentOffset + this->memory.size());
		Dod::DataUtils::initFromMemory(this->table, expectCapacityEls, memSpan);
		EXPECT_EQ(Dod::DataUtils::getCapacity(this->table), expectCapacityEls);
	}

	void pushBack(int32_t expectedNumOfFilledEls, auto&& ... values)
	{
		Dod::DataUtils::pushBack(this->table, true, std::forward<decltype(values)>(values)...);
	}

	alignas(alignment) std::array<Dod::MemTypes::data_t, memorySize + memoryAlignmentOffset> memory{};
	Dod::DTable<Types...> table;

};

using AppendIntFloat = TableTest<bytesForIntFloat7, 0, int, float>;
TEST_F(AppendIntFloat, Append2Elements)
{

	TableToAppend<bytesForIntFloat3, 0, int, float> srcTable(3);

	this->init(7);
	srcTable.pushBack(1, 1, 2.f);
	srcTable.pushBack(2, 3, 4.f);

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	this->checkTable<int, float>({ {1, 3}, {2.f, 4.f} });

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, float>({ {1, 3, 1, 3}, {2.f, 4.f, 2.f, 4.f} });

}

TEST_F(AppendIntFloat, Append3Elements)
{

	TableToAppend<bytesForIntFloat3, 0, int, float> srcTable(3);

	this->init(7);
	srcTable.pushBack(1, 1, 2.f);
	srcTable.pushBack(2, 3, 4.f);
	srcTable.pushBack(3, 5, 6.f);

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 3);
	this->checkTable<int, float>({ {1, 3, 5}, {2.f, 4.f, 6.f} });

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 6);
	this->checkTable<int, float>({ {1, 3, 5, 1, 3, 5}, {2.f, 4.f, 6.f, 2.f, 4.f, 6.f} });

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 7);
	this->checkTable<int, float>({ {1, 3, 5, 1, 3, 5, 1}, {2.f, 4.f, 6.f, 2.f, 4.f, 6.f, 2.f} });

}

TEST_F(AppendIntFloat, Append0Elements)
{

	TableToAppend<bytesForIntFloat3, 0, int, float> srcTable(3);

	this->init(7);

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	{
		auto [ints, floats] { Dod::DataUtils::get(this->table) };
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(ints), 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(floats), 0);
	}

}

using AppendIntDouble = TableTest<bytesForIntDouble4, 0, int, double>;
TEST_F(AppendIntDouble, Append3Elements)
{

	TableToAppend<bytesForIntDouble2, 0, int, double> srcTable(2);

	this->init(4);
	srcTable.pushBack(1, 1, 2.);
	srcTable.pushBack(2, 3, 4.);

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	this->checkTable<int, double>({ {1, 3}, {2., 4.} });

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, double>({ {1, 3, 1, 3}, {2., 4., 2., 4.} });

	Dod::DataUtils::append(this->table, Dod::ImTable(srcTable.table));
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, double>({ {1, 3, 1, 3}, {2., 4., 2., 4.} });

}

using FlushIntFloat = TableTest<bytesForIntFloat3, 0, int, float>;
TEST_F(FlushIntFloat, FlushAllElements)
{

	this->init(3);
	this->pushBack(1, 1, 2.f);
	this->checkTable<int, float>({ {1}, {2.f} });
	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);

	this->pushBack(1, 3, 4.f);
	this->pushBack(2, 5, 6.f);
	this->pushBack(3, 7, 8.f);
	this->pushBack(3, 9, 10.f);
	this->checkTable<int, float>({ {3, 5, 7}, {4.f, 6.f, 8.f} });

	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	this->pushBack(1, 11, 12.f);
	this->pushBack(2, 13, 14.f);
	this->checkTable<int, float>({ {11, 13}, {12.f, 14.f} });

}

using RemoveElementsIntFloat = TableTest<bytesForIntFloat3, 0, int, float>;
TEST_F(RemoveElementsIntFloat, RemoveElements)
{

	this->init(3);

	this->pushBack(1, 1, 2.f);
	this->pushBack(2, 3, 4.f);
	this->remove(1, { 1 });
	this->checkTable<int, float>({ { 1 }, { 2.f } });

	this->remove(0, { 0 });

	this->pushBack(1, 5, 6.f);
	this->pushBack(2, 7, 8.f);
	this->remove(1, { 0 });
	this->checkTable<int, float>({ { 7 }, { 8.f } });

	this->remove(0, { 0 });

	this->pushBack(1, 9, 10.f);
	this->pushBack(2, 11, 12.f);
	this->remove(0, { 0, 1 });

	this->pushBack(1, 13, 14.f);
	this->pushBack(2, 15, 16.f);
	this->pushBack(3, 17, 18.f);
	this->remove(1, { 0, 2 });
	this->checkTable<int, float>({ { 15 }, { 16.f } });

	this->pushBack(2, 19, 20.f);
	this->pushBack(3, 21, 22.f);
	this->checkTable<int, float>({ { 15, 19, 21 }, { 16.f, 20.f, 22.f } });

	this->remove(3, { });
	this->checkTable<int, float>({ { 15, 19, 21 }, { 16.f, 20.f, 22.f } });

	Dod::DataUtils::flush(this->table);

}

TEST(DataUtils, CreateGuidedImTable)
{

	using type_t = int32_t;
	alignas(alignment) std::array<type_t, 8> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	Dod::ImTable<int32_t> table;

	struct Span
	{
		Dod::MemTypes::dataConstPoint_t dataBegin;
		Dod::MemTypes::dataConstPoint_t dataEnd;
	};
	const auto numOfElements{ static_cast<int32_t>(values.size()) };
	Dod::DataUtils::initFromMemory(table, numOfElements, numOfElements, Span(
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data()),
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data() + 64)
	));

	alignas(alignment) std::array<type_t, values.size()> indices{ {7, 6, 5, 4, 3, 2, 1, 0} };
	Dod::ImTable<int32_t> indicesBuffer;
	Dod::DataUtils::initFromMemory(indicesBuffer, static_cast<int32_t>(indices.size()), static_cast<int32_t>(indices.size()), Span(
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(indices.data()),
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(indices.data() + 64)
	));

	Dod::ImTableGuided<int32_t> guidedTable{ Dod::DataUtils::createGuidedImTable(table, indicesBuffer) };

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(guidedTable), 8);
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 0), static_cast<type_t>(8));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 1), static_cast<type_t>(7));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 2), static_cast<type_t>(6));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 3), static_cast<type_t>(5));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 4), static_cast<type_t>(4));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 5), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 6), static_cast<type_t>(2));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 7), static_cast<type_t>(1));

}

TEST(DataUtils, CreateGuidedImTableStarving)
{

	using type_t = int32_t;
	alignas(alignment) std::array<type_t, 8> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	Dod::ImTable<int32_t> table;

	struct Span
	{
		Dod::MemTypes::dataConstPoint_t dataBegin;
		Dod::MemTypes::dataConstPoint_t dataEnd;
	};
	const auto numOfElements{ static_cast<int32_t>(values.size()) };
	Dod::DataUtils::initFromMemory(table, numOfElements, numOfElements, Span(
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data()),
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data() + 64)
	));

	alignas(alignment) std::array<type_t, 5> indices{ {1, 3, 2, 2, 7} };
	Dod::ImTable<int32_t> indicesBuffer;
	Dod::DataUtils::initFromMemory(indicesBuffer, static_cast<int32_t>(indices.size()), static_cast<int32_t>(indices.size()), Span(
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(indices.data()),
		reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(indices.data() + 64)
	));

	Dod::ImTableGuided<int32_t> guidedTable{ Dod::DataUtils::createGuidedImTable(table, indicesBuffer) };

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(guidedTable), 5);
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 0), static_cast<type_t>(2));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 1), static_cast<type_t>(4));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 2), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 3), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DataUtils::get(guidedTable, 4), static_cast<type_t>(8));

}

TEST(DataUtils, ForEachElement)
{

	{
		using type_t = int32_t;
		alignas(alignment) auto values{ std::to_array<type_t>({1, 2, 3, 4, 5, 6, 7, 8}) };

		Dod::ImTable<type_t> table;

		struct Span
		{
			Dod::MemTypes::dataConstPoint_t dataBegin;
			Dod::MemTypes::dataConstPoint_t dataEnd;
		};
		Dod::DataUtils::initFromMemory(table, static_cast<type_t>(values.size()), static_cast<type_t>(values.size()), Span(
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data()),
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data() + 64)
		));

		std::vector<type_t> result;
		Dod::DataUtils::forEach(table, [&result](type_t el) {
			result.push_back(el);
		});
		EXPECT_TRUE(std::equal(result.begin(), result.end(), values.begin(), values.end()));

	}

	{
		using type_t = double;
		alignas(alignment) auto values{ std::to_array<type_t>({0.1, 0.5, 1.5, -42.}) };

		Dod::ImTable<type_t> table;

		struct Span
		{
			Dod::MemTypes::dataConstPoint_t dataBegin;
			Dod::MemTypes::dataConstPoint_t dataEnd;
		};
		Dod::DataUtils::initFromMemory(table, static_cast<type_t>(values.size()), static_cast<type_t>(values.size()), Span(
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data()),
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(values.data() + 64)
		));

		std::vector<type_t> result;
		Dod::DataUtils::forEach(table, [&result](type_t el) {
			result.push_back(el);
		});
		EXPECT_TRUE(std::equal(result.begin(), result.end(), values.begin(), values.end()));

	}

	{
		using type_t = double;
		alignas(alignment) auto values{ std::to_array<type_t>({0.1, 0.5, 1.5, -42.}) };

		Dod::MutTable<type_t> table;

		struct Span
		{
			Dod::MemTypes::dataPoint_t dataBegin;
			Dod::MemTypes::dataPoint_t dataEnd;
		};
		Dod::DataUtils::initFromMemory(table, static_cast<type_t>(values.size()), static_cast<type_t>(values.size()), Span(
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(values.data()),
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(values.data() + 64)
		));

		std::vector<type_t> result;
		Dod::DataUtils::forEach(table, [&result](type_t el) {
			result.push_back(el);
		});
		EXPECT_TRUE(std::equal(result.begin(), result.end(), values.begin(), values.end()));

	}

	{
		using type_t = int32_t;
		alignas(alignment) auto values{ std::to_array<type_t>( {1, 2, 3, 4, 5, 6, 7, 8}) };

		Dod::MutTable<type_t> table;

		struct Span
		{
			Dod::MemTypes::dataPoint_t dataBegin;
			Dod::MemTypes::dataPoint_t dataEnd;
		};
		Dod::DataUtils::initFromMemory(table, static_cast<type_t>(values.size()), static_cast<type_t>(values.size()), Span(
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(values.data()),
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(values.data() + 64)
		));

		Dod::DataUtils::forEach(table, [](type_t& el) {
			el *= 10;
		});

		const auto expectedValues{ std::to_array<type_t>({10, 20, 30, 40, 50, 60, 70, 80}) };
		EXPECT_TRUE(std::equal(expectedValues.begin(), expectedValues.end(), values.begin(), values.end()));

	}

}
