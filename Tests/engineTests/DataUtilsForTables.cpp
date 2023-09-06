#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/DataUtils.h>
#include <dod/Tables.h>

#include "utils/CommonStructures.h"

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#include <array>
#pragma warning(pop)

struct NonTrivialType
{

	NonTrivialType(std::vector<int> initial)
		:data(initial)
	{
		++this->counter;
	}
	NonTrivialType(const NonTrivialType& other)
	{
		this->data = other.data;
		++this->counter;
	}
	~NonTrivialType()
	{
		--this->counter;
	}

	[[nodiscard]] NonTrivialType& operator=(const NonTrivialType& other)
	{
		this->data = other.data;
		++this->counter;
		return *this;
	}

	static int counter;
	std::vector<int> data;

	[[nodiscard]] friend auto operator<=>(const NonTrivialType&, const NonTrivialType&) = default;
};
int NonTrivialType::counter = 0;

static_assert(std::is_trivial_v<NonTrivialType> == false);
static_assert(std::is_move_assignable_v<NonTrivialType> == true);
static_assert(std::is_move_constructible_v<NonTrivialType> == true);
static_assert(std::is_copy_assignable_v<NonTrivialType> == true);
static_assert(std::is_copy_constructible_v<NonTrivialType> == true);
static_assert(sizeof(NonTrivialType) == 32);

struct NonTrivialMoveOnlyType
{
	std::vector<int> data;
	static int counter;

	NonTrivialMoveOnlyType(std::vector<int> data) noexcept
		:data(data)
	{
		++this->counter;
	}

	NonTrivialMoveOnlyType(NonTrivialMoveOnlyType&& other) noexcept
		:data(std::move(other.data))
	{
		++this->counter;
	}

	[[nodiscard]] NonTrivialMoveOnlyType& operator=(NonTrivialMoveOnlyType&& other) noexcept
	{
		std::exchange(this->data, other.data);
		++this->counter;
	};

	~NonTrivialMoveOnlyType()
	{
		--this->counter;
	}

	NonTrivialMoveOnlyType(const NonTrivialMoveOnlyType&) noexcept = delete;
	[[nodiscard]] NonTrivialMoveOnlyType& operator=(const NonTrivialMoveOnlyType&) noexcept = delete;


	[[nodiscard]] friend auto operator<=>(const NonTrivialMoveOnlyType&, const NonTrivialMoveOnlyType&) = default;
};
int NonTrivialMoveOnlyType::counter = 0;


static_assert(std::is_trivial_v<NonTrivialMoveOnlyType> == false);
static_assert(std::is_move_assignable_v<NonTrivialMoveOnlyType> == true);
static_assert(std::is_move_constructible_v<NonTrivialMoveOnlyType> == true);
static_assert(std::is_copy_assignable_v<NonTrivialMoveOnlyType> == false);
static_assert(std::is_copy_constructible_v<NonTrivialMoveOnlyType> == false);
static_assert(sizeof(NonTrivialMoveOnlyType) == 32);

template <size_t memorySize, typename ... Types>
class InitDBTableFromMemoryTest : public ::testing::Test {

protected:
	void run(int32_t memBeginIndex, int32_t memEndIndex, int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan, memBeginIndex, memEndIndex);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table), 0);

		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(this->table.dataBegin), memSpan.dataBegin + memBeginIndex);
		EXPECT_EQ(Dod::DataUtils::getCapacity(this->table), expectCapacityEls);

	}

	void run(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table), 0);

		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(this->table.dataBegin), memSpan.dataBegin);
		EXPECT_EQ(Dod::DataUtils::getCapacity(this->table), expectCapacityEls);

	}

	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using InitDBTableFromMemory32BytesInt = InitDBTableFromMemoryTest<32, int>;
TEST_F(InitDBTableFromMemory32BytesInt, InitFromMemory)
{

	this->run(7);

	this->run(0, 32, 7);
	this->run(0, 28, 6);
	this->run(0, 8, 1);
	this->run(0, 4, 0);
	this->run(0, 2, 0);
	this->run(0, 0, 0);

	this->run(4, 32, 6);
	this->run(24, 32, 1);
	this->run(28, 32, 0);
	this->run(30, 32, 0);

}

using InitDBTableFromMemory32BytesIntFloat = InitDBTableFromMemoryTest<32, int, float>;
TEST_F(InitDBTableFromMemory32BytesIntFloat, InitFromMemory)
{

	this->run(3);

	this->run(0, 32, 3);
	this->run(0, 20, 2);
	this->run(0, 12, 1);
	this->run(0, 8, 0);

	this->run(12, 32, 2);
	this->run(22, 32, 0);
	this->run(30, 32, 0);

}

using InitDBTableFromMemory32BytesIntDouble = InitDBTableFromMemoryTest<32, int, double>;
TEST_F(InitDBTableFromMemory32BytesIntDouble, InitFromMemory)
{

	this->run(2);

	this->run(0, 32, 2);
	this->run(0, 30, 1);
	this->run(0, 12, 0);
	this->run(0, 4, 0);

}

template <size_t memorySize, typename ... Types>
class TableTest : public ::testing::Test {
	using types_t = std::tuple<Types...>;
	static constexpr auto deadBucketSize{ Dod::DataUtils::computeDeadbucketSizeInBytes<types_t>() };
protected:
	void init(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan);
		ASSERT_EQ(Dod::DataUtils::getCapacity(this->table), expectCapacityEls);

	}

	void populate(int32_t expectedNumOfFilledEls, auto&& ... values)
	{
		Dod::DataUtils::populate(this->table, true, std::forward<decltype(values)>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	template <typename T>
	void checkInternal(std::vector<T> expectedValues, int32_t offset)
	{
		const auto populatedPtr{ reinterpret_cast<T*>(table.dataBegin + deadBucketSize + static_cast<size_t>(offset)) };

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(*(populatedPtr + (id++)), expected);
	}

	template <int32_t columnId, typename T>
	void check(std::vector<T>&& expectedValues)
	{
		Dod::ImBuffer<T> col{ Dod::DataUtils::get<columnId>(this->table) };
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(col), expectedValues.size());

		for (int32_t id{}; auto&& expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(col, id++), expected);
	}

	template <typename ... T>
	void checkTable(std::tuple<std::vector<T>...>&& expectedValues)
	{

		const auto columns{ Dod::DataUtils::get(this->table) };

		RisingCore::Helpers::constexprLoop<std::tuple_size_v<std::decay_t<decltype(expectedValues)>>>([&]<size_t columnId>() {
			auto&& expectedColumn{ std::get<columnId>(expectedValues) };
			this->check<columnId>(std::move(expectedColumn));
		});

	}

	void remove(int32_t expectedEls, std::vector<int32_t> indicesMem)
	{
		Dod::ImBuffer<int32_t> indicesToRemove;
		Dod::DataUtils::initFromArray(indicesToRemove, indicesMem);
		Dod::DataUtils::remove(this->table, indicesToRemove);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), expectedEls);
	}

	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using PopulateIntFloat = TableTest<32, int, float>;
TEST_F(PopulateIntFloat, Populate)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	this->checkInternal<int>({ 1 }, 0);
	this->checkInternal<float>({ 2.f }, 12);

	this->populate(2, 3, 4.f);
	this->checkInternal<int>({ 1, 3 }, 0);
	this->checkInternal<float>({ 2.f, 4.f }, 12);

	this->populate(3, 5, 6.f);
	this->checkInternal<int>({ 1, 3, 5 }, 0);
	this->checkInternal<float>({ 2.f, 4.f, 6 }, 12);

	this->populate(3, 7, 8.f);

}

using PopulateIntDouble = TableTest<32, int, double>;
TEST_F(PopulateIntDouble, Populate)
{

	this->init(2);

	this->populate(1, 1, 2.);
	this->checkInternal<int>({ 1 }, 0);
	this->checkInternal<double>({ 2. }, 8);

	this->populate(2, 3, 4.);
	this->checkInternal<int>({ 1, 3 }, 0);
	this->checkInternal<double>({ 2., 4. }, 8);

	this->populate(2, 5, 6.);

}

using PopulateDoubleInt = TableTest<32, double, int>;
TEST_F(PopulateDoubleInt, Populate)
{

	this->init(2);

	this->populate(1, 1., 2);
	this->checkInternal<double>({ 1. }, 0);
	this->checkInternal<int>({ 2 }, 16);

	this->populate(2, 3., 4);
	this->checkInternal<double>({ 1., 3. }, 0);
	this->checkInternal<int>({ 2, 4 }, 16);

	this->populate(2, 5., 6);

}

using PopulateIntNonTrivial = TableTest<128, int, NonTrivialType>;
TEST_F(PopulateIntNonTrivial, Populate)
{

	this->init(2);
	NonTrivialType::counter = 0;

	NonTrivialType nonTrivial1({ 10, 20 });
	this->populate(1, 1, nonTrivial1);
	EXPECT_EQ(NonTrivialType::counter, 2);
	this->check<0, int>({ 1 });
	this->check<1, NonTrivialType>({ nonTrivial1 });

	NonTrivialType nonTrivial2({ 30, 40, 50 });
	this->populate(2, 3, nonTrivial2);
	EXPECT_EQ(NonTrivialType::counter, 4);
	this->check<0, int>({ 1, 3 });
	this->check<1, NonTrivialType>({ nonTrivial1, nonTrivial2 });

	NonTrivialType nonTrivial3({ 60, 70 });
	this->populate(2, 5, nonTrivial3);
	EXPECT_EQ(NonTrivialType::counter, 5);
	this->check<0, int>({ 1, 3 });
	this->check<1, NonTrivialType>({ nonTrivial1, nonTrivial2 });

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

using PopulateIntNonTrivialMoveOnly = TableTest<128, int, NonTrivialMoveOnlyType>;
TEST_F(PopulateIntNonTrivialMoveOnly, Populate)
{

	this->init(2);
	NonTrivialMoveOnlyType::counter = 0;

	const auto genNonTrivial1 = []() noexcept {
		return NonTrivialMoveOnlyType({ 10, 20 });
	};
	const auto genNonTrivial2 = []() noexcept {
		return NonTrivialMoveOnlyType({ 30, 40, 50 });
	};
	const auto genNonTrivial3 = []() noexcept {
		return NonTrivialMoveOnlyType({ 60, 70 });
	};

	this->populate(1, 1, genNonTrivial1());
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 1);
	this->check<0, int>({ 1 });
	this->check<1, NonTrivialMoveOnlyType>(genCheckVector(genNonTrivial1()));

	this->populate(2, 3, genNonTrivial2());
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 2);
	this->check<0, int>({ 1, 3 });
	this->check<1, NonTrivialMoveOnlyType>(genCheckVector(genNonTrivial1(), genNonTrivial2()));

	this->populate(2, 5, genNonTrivial3());
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 2);
	this->check<0, int>({ 1, 3 });
	this->check<1, NonTrivialMoveOnlyType>(genCheckVector(genNonTrivial1(), genNonTrivial2()));

}

using GetIntFloat = TableTest<32, int, float>;
TEST_F(GetIntFloat, GetElements)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	this->check<0, int>({ 1 });
	this->check<1, float>({ 2.f });

	this->populate(2, 3, 4.f);
	this->check<0, int>({ 1, 3 });
	this->check<1, float>({ 2.f, 4.f });

	this->populate(3, 5, 6.f);
	this->check<0, int>({ 1, 3, 5 });
	this->check<1, float>({ 2.f, 4.f, 6.f });

	this->populate(3, 7, 8.f);

}

using GetIntDouble = TableTest<32, int, double>;
TEST_F(GetIntDouble, GetElements)
{

	this->init(2);

	this->populate(1, 1, 2.);
	this->check<0, int>({ 1 });
	this->check<1, double>({ 2. });

	this->populate(2, 3, 4.);
	this->check<0, int>({ 1, 3 });
	this->check<1, double>({ 2., 4. });

	this->populate(2, 5, 6.f);

}

using GetAllIntFloat = TableTest<32, int, float>;
TEST_F(GetAllIntFloat, GetAllElements)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	this->checkTable<int, float>({ {1}, {2.f} });

	this->populate(2, 3, 4.f);
	this->checkTable<int, float>({ {1, 3}, {2.f, 4.f} });

	this->populate(3, 5, 6.f);
	this->checkTable<int, float>({ {1, 3, 5}, {2.f, 4.f, 6.f} });

	this->populate(3, 7, 8.f);

}

using GetAllIntDouble = TableTest<32, int, double>;
TEST_F(GetAllIntDouble, GetAllElements)
{

	this->init(2);

	this->populate(1, 1, 2.);
	this->checkTable<int, double>({ {1}, {2.} });

	this->populate(2, 3, 4.);
	this->checkTable<int, double>({ {1, 3}, {2., 4.} });

	this->populate(2, 5, 6.f);

}

template <size_t memorySize, typename ... Types>
class TableToAppend
{
public:
	TableToAppend(int32_t expectCapacityEls)
	{
		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());
		Dod::DataUtils::initFromMemory(this->table, memSpan);
	}

	void populate(int32_t expectedNumOfFilledEls, auto&& ... values)
	{
		Dod::DataUtils::populate(this->table, true, std::forward<Types>(values)...);
	}

	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using AppendIntFloat = TableTest<64, int, float>;
TEST_F(AppendIntFloat, Append2Elements)
{

	TableToAppend<32, int, float> srcTable(3);

	this->init(7);
	srcTable.populate(1, 1, 2.f);
	srcTable.populate(2, 3, 4.f);

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	this->checkTable<int, float>({ {1, 3}, {2.f, 4.f} });

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, float>({ {1, 3, 1, 3}, {2.f, 4.f, 2.f, 4.f} });

}

TEST_F(AppendIntFloat, Append3Elements)
{

	TableToAppend<32, int, float> srcTable(3);

	this->init(7);
	srcTable.populate(1, 1, 2.f);
	srcTable.populate(2, 3, 4.f);
	srcTable.populate(3, 5, 6.f);

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 3);
	this->checkTable<int, float>({ {1, 3, 5}, {2.f, 4.f, 6.f} });

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 6);
	this->checkTable<int, float>({ {1, 3, 5, 1, 3, 5}, {2.f, 4.f, 6.f, 2.f, 4.f, 6.f} });

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 7);
	this->checkTable<int, float>({ {1, 3, 5, 1, 3, 5, 1}, {2.f, 4.f, 6.f, 2.f, 4.f, 6.f, 2.f} });

}

TEST_F(AppendIntFloat, Append0Elements)
{

	TableToAppend<32, int, float> srcTable(3);

	this->init(7);

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(ints), 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(floats), 0);
	}

}

using AppendIntDouble = TableTest<64, int, double>;
TEST_F(AppendIntDouble, Append3Elements)
{

	TableToAppend<32, int, double> srcTable(2);

	this->init(4);
	srcTable.populate(1, 1, 2.);
	srcTable.populate(2, 3, 4.);

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	this->checkTable<int, double>({ {1, 3}, {2., 4.} });

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, double>({ {1, 3, 1, 3}, {2., 4., 2., 4.} });

	Dod::DataUtils::append(this->table, srcTable.table);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, double>({ {1, 3, 1, 3}, {2., 4., 2., 4.} });

}

using AppendIntNonTrivial = TableTest<176, int, NonTrivialType>;
TEST_F(AppendIntNonTrivial, Append2Elements)
{

	TableToAppend<140, int, NonTrivialType> srcTable(3);

	this->init(4);
	NonTrivialType::counter = 0;

	NonTrivialType nonTrivial1({ 10, 20 });
	srcTable.populate(1, 1, nonTrivial1);
	NonTrivialType nonTrivial2({ 30, 40 });
	srcTable.populate(2, 3, nonTrivial2);

	Dod::DataUtils::append(this->table, srcTable.table);
	EXPECT_EQ(NonTrivialType::counter, 6);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	this->checkTable<int, NonTrivialType>({ {1, 3}, {nonTrivial1, nonTrivial2} });

	Dod::DataUtils::append(this->table, srcTable.table);
	EXPECT_EQ(NonTrivialType::counter, 8);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, NonTrivialType>({ {1, 3, 1, 3}, {nonTrivial1, nonTrivial2, nonTrivial1, nonTrivial2} });

	Dod::DataUtils::append(this->table, srcTable.table);
	EXPECT_EQ(NonTrivialType::counter, 8);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable<int, NonTrivialType>({ {1, 3, 1, 3}, {nonTrivial1, nonTrivial2, nonTrivial1, nonTrivial2} });

}

using AppendIntNonTrivialMoveOnly = TableTest<176, int, NonTrivialMoveOnlyType>;
TEST_F(AppendIntNonTrivialMoveOnly, Append2Elements)
{


	this->init(4);
	NonTrivialMoveOnlyType::counter = 0;

	const auto genNonTrivial1 = []() noexcept {
		return NonTrivialMoveOnlyType({ 10, 20 });
	};
	const auto genNonTrivial2 = []() noexcept {
		return NonTrivialMoveOnlyType({ 30, 400 });
	};

	TableToAppend<140, int, NonTrivialMoveOnlyType> srcTable1(3);
	srcTable1.populate(1, 1, genNonTrivial1());
	srcTable1.populate(2, 3, genNonTrivial2());

	Dod::DataUtils::append(this->table, srcTable1.table);
	// Elements are moved, but not destroyed.
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 4);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	this->checkTable(genCheckTuple<int, NonTrivialMoveOnlyType>({1, 3}, genCheckVector(genNonTrivial1(), genNonTrivial2())));

	TableToAppend<140, int, NonTrivialMoveOnlyType> srcTable2(3);
	srcTable2.populate(1, 1, genNonTrivial1());
	srcTable2.populate(2, 3, genNonTrivial2());

	Dod::DataUtils::append(this->table, srcTable2.table);
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 8);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable(genCheckTuple<int, NonTrivialMoveOnlyType>({1, 3, 1, 3}, genCheckVector(genNonTrivial1(), genNonTrivial2(), genNonTrivial1(), genNonTrivial2())));

	TableToAppend<140, int, NonTrivialMoveOnlyType> srcTable3(3);
	srcTable3.populate(1, 1, genNonTrivial1());
	srcTable3.populate(2, 3, genNonTrivial2());

	Dod::DataUtils::append(this->table, srcTable3.table);
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 10);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	this->checkTable(genCheckTuple<int, NonTrivialMoveOnlyType>({1, 3, 1, 3}, genCheckVector(genNonTrivial1(), genNonTrivial2(), genNonTrivial1(), genNonTrivial2())));

}

using FlushIntFloat = TableTest<32, int, float>;
TEST_F(FlushIntFloat, FlushAllElements)
{

	this->init(3);
	this->populate(1, 1, 2.f);
	this->checkTable<int, float>({ {1}, {2.f} });
	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);

	this->populate(1, 3, 4.f);
	this->populate(2, 5, 6.f);
	this->populate(3, 7, 8.f);
	this->populate(3, 9, 10.f);
	this->checkTable<int, float>({ {3, 5, 7}, {4.f, 6.f, 8.f} });

	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	this->populate(1, 11, 12.f);
	this->populate(2, 13, 14.f);
	this->checkTable<int, float>({ {11, 13}, {12.f, 14.f} });

}

using FlushIntNonTrivial = TableTest<140, int, NonTrivialType>;
TEST_F(FlushIntNonTrivial, FlushAllElements)
{

	this->init(3);
	NonTrivialType::counter = 0;
	
	NonTrivialType nonTrivial1({ 10, 20 });
	this->populate(1, 1, nonTrivial1);
	EXPECT_EQ(NonTrivialType::counter, 2);
	this->checkTable<int, NonTrivialType>({ {1}, {nonTrivial1} });
	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(NonTrivialType::counter, 1);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);

	NonTrivialType nonTrivial2({ 30, 40 });
	this->populate(1, 3, nonTrivial2);
	NonTrivialType nonTrivial3({ 50, 60 });
	this->populate(2, 5, nonTrivial3);
	NonTrivialType nonTrivial4({ 70, 80 });
	this->populate(3, 7, nonTrivial4);
	NonTrivialType nonTrivial5({ 90, 100 });
	this->populate(3, 9, nonTrivial5);
	EXPECT_EQ(NonTrivialType::counter, 8);
	this->checkTable<int, NonTrivialType>({ {3, 5, 7}, {nonTrivial2, nonTrivial3, nonTrivial4} });

	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(NonTrivialType::counter, 5);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	NonTrivialType nonTrivial6({ 110, 120 });
	this->populate(1, 11, nonTrivial6);
	NonTrivialType nonTrivial7({ 130, 140 });
	this->populate(2, 13, nonTrivial7);
	EXPECT_EQ(NonTrivialType::counter, 9);
	this->checkTable<int, NonTrivialType>({ {11, 13}, {nonTrivial6, nonTrivial7} });

}

using FlushIntNonTrivialMoveOnly = TableTest<140, int, NonTrivialMoveOnlyType>;
TEST_F(FlushIntNonTrivialMoveOnly, FlushAllElements)
{

	this->init(3);
	NonTrivialMoveOnlyType::counter = 0;


	const auto genNonTrivial1 = []() noexcept {	return NonTrivialMoveOnlyType({ 10, 20 }); };
	const auto genNonTrivial2 = []() noexcept {	return NonTrivialMoveOnlyType({ 30, 40 }); };
	const auto genNonTrivial3 = []() noexcept {	return NonTrivialMoveOnlyType({ 50, 60 }); };
	const auto genNonTrivial4 = []() noexcept {	return NonTrivialMoveOnlyType({ 70, 80 }); };
	const auto genNonTrivial5 = []() noexcept {	return NonTrivialMoveOnlyType({ 90, 100 }); };
	const auto genNonTrivial6 = []() noexcept {	return NonTrivialMoveOnlyType({ 110, 120 }); };
	const auto genNonTrivial7 = []() noexcept {	return NonTrivialMoveOnlyType({ 130, 140 }); };

	this->populate(1, 1, genNonTrivial1());
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 1);
	this->checkTable(genCheckTuple<int, NonTrivialMoveOnlyType>({1}, genCheckVector(genNonTrivial1())));
	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 0);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);

	this->populate(1, 3, genNonTrivial2());
	this->populate(2, 5, genNonTrivial3());
	this->populate(3, 7, genNonTrivial4());
	this->populate(3, 9, genNonTrivial5());
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 3);
	this->checkTable(genCheckTuple<int, NonTrivialMoveOnlyType>({3, 5, 7}, genCheckVector(genNonTrivial2(), genNonTrivial3(), genNonTrivial4())));

	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 0);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	this->populate(1, 11, genNonTrivial6());
	this->populate(2, 13, genNonTrivial7());
	EXPECT_EQ(NonTrivialMoveOnlyType::counter, 2);
	this->checkTable(genCheckTuple<int, NonTrivialMoveOnlyType>({11, 13}, genCheckVector(genNonTrivial6(), genNonTrivial7())));

}

using RemoveElementsIntFloat = TableTest<32, int, float>;
TEST_F(RemoveElementsIntFloat, RemoveElements)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	this->populate(2, 3, 4.f);
	this->remove(1, { 1 });
	this->checkTable<int, float>({ { 1 }, { 2.f } });

	this->remove(0, { 0 });

	this->populate(1, 5, 6.f);
	this->populate(2, 7, 8.f);
	this->remove(1, { 0 });
	this->checkTable<int, float>({ { 7 }, { 8.f } });

	this->remove(0, { 0 });

	this->populate(1, 9, 10.f);
	this->populate(2, 11, 12.f);
	this->remove(0, { 0, 1 });

	this->populate(1, 13, 14.f);
	this->populate(2, 15, 16.f);
	this->populate(3, 17, 18.f);
	this->remove(1, { 0, 2 });
	this->checkTable<int, float>({ { 15 }, { 16.f } });

	this->populate(2, 19, 20.f);
	this->populate(3, 21, 22.f);
	this->checkTable<int, float>({ { 15, 19, 21 }, { 16.f, 20.f, 22.f } });

	this->remove(3, { });
	this->checkTable<int, float>({ { 15, 19, 21 }, { 16.f, 20.f, 22.f } });

}
