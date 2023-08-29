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

template <size_t memorySize, typename ... Types>
class InitDBTableFromMemoryTest : public ::testing::Test {

protected:
	void run(int32_t memBeginIndex, int32_t memEndIndex, int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan, memBeginIndex, memEndIndex);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table), 0);

		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(this->table.dataBegin), memSpan.dataBegin + memBeginIndex);
		EXPECT_EQ(this->table.capacityEls, expectCapacityEls);

	}

	void run(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table), 0);

		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(this->table.dataBegin), memSpan.dataBegin);
		EXPECT_EQ(this->table.capacityEls, expectCapacityEls);

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
class PopulateTest : public ::testing::Test {

	using types_t = std::tuple<Types...>;
	static constexpr auto deadBucketSize{ Dod::DataUtils::computeDeadbucketSizeInBytes<types_t>() };

protected:
	void init(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan);
		ASSERT_EQ(this->table.capacityEls, expectCapacityEls);

	}

	void populate(int32_t expectedNumOfFilledEls, Types&& ... values)
	{
		Dod::DataUtils::populate<Types...>(this->table, true, std::forward<Types>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	template <typename T>
	void check(std::vector<T> expectedValues, int32_t offset)
	{
		const auto populatedPtr{ reinterpret_cast<T*>(table.dataBegin + deadBucketSize + static_cast<size_t>(offset)) };

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(*(populatedPtr + (id++)), expected);
	}

	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using PopulateIntFloat = PopulateTest<32, int, float>;
TEST_F(PopulateIntFloat, Populate)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	this->check<int>({ 1 }, 0);
	this->check<float>({ 2.f }, 12);

	this->populate(2, 3, 4.f);
	this->check<int>({ 1, 3 }, 0);
	this->check<float>({ 2.f, 4.f }, 12);

	this->populate(3, 5, 6.f);
	this->check<int>({ 1, 3, 5 }, 0);
	this->check<float>({ 2.f, 4.f, 6 }, 12);

	this->populate(3, 7, 8.f);

}

using PopulateIntDouble = PopulateTest<32, int, double>;
TEST_F(PopulateIntDouble, Populate)
{

	this->init(2);

	this->populate(1, 1, 2.);
	this->check<int>({ 1 }, 0);
	this->check<double>({ 2. }, 8);

	this->populate(2, 3, 4.);
	this->check<int>({ 1, 3 }, 0);
	this->check<double>({ 2., 4. }, 8);

	this->populate(2, 5, 6.);

}

using PopulateDoubleInt = PopulateTest<32, double, int>;
TEST_F(PopulateDoubleInt, Populate)
{

	this->init(2);

	this->populate(1, 1., 2);
	this->check<double>({ 1. }, 0);
	this->check<int>({ 2 }, 16);

	this->populate(2, 3., 4);
	this->check<double>({ 1., 3. }, 0);
	this->check<int>({ 2, 4 }, 16);

	this->populate(2, 5., 6);

}

template <size_t memorySize, typename ... Types>
class GetElementsTest : public ::testing::Test {

protected:
	void init(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan);
		ASSERT_EQ(this->table.capacityEls, expectCapacityEls);

	}

	void populate(int32_t expectedNumOfFilledEls, Types&& ... values)
	{
		Dod::DataUtils::populate<Types...>(this->table, true, std::forward<Types>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	template <int32_t columnId, typename T>
	void check(std::vector<T> expectedValues)
	{
		Dod::ImBuffer<T> col{ Dod::DataUtils::get<columnId>(this->table) };
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(col), expectedValues.size());
		
		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(col, id++), expected);
	}
	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using GetIntFloat = GetElementsTest<32, int, float>;
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

using GetIntDouble = GetElementsTest<32, int, double>;
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

template <size_t memorySize, typename ... Types>
class GetAllElementsTest : public ::testing::Test {

protected:
	void init(int32_t expectCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());

		Dod::DataUtils::initFromMemory(this->table, memSpan);
		ASSERT_EQ(this->table.capacityEls, expectCapacityEls);

	}

	void populate(int32_t expectedNumOfFilledEls, Types&& ... values)
	{
		Dod::DataUtils::populate<Types...>(this->table, true, std::forward<Types>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), expectedNumOfFilledEls);
	}

	template <int32_t columnId, typename T>
	void check(std::vector<T> expectedValues)
	{
		Dod::ImBuffer<T> col{ Dod::DataUtils::get<columnId>(this->table) };
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(col), expectedValues.size());

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(col, id++), expected);
	}
	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using GetAllIntFloat = GetAllElementsTest<32, int, float>;
TEST_F(GetAllIntFloat, GetAllElements)
{

	this->init(3);

	this->populate(1, 1, 2.f);

	const auto [ints, floats] { Dod::DataUtils::get(this->table) };

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(ints), 1);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(float), 1);

}