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

	template <typename T>
	void check(auto&& buffer, std::vector<T> expectedValues)
	{
		static_assert(std::is_same_v<std::decay_t<decltype(buffer)>, Dod::ImBuffer<T>>);

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(buffer), expectedValues.size());

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(buffer, id++), expected);
	}
	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using GetAllIntFloat = GetAllElementsTest<32, int, float>;
TEST_F(GetAllIntFloat, GetAllElements)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1 });
		this->check<float>(floats, { 2.f });
	}

	this->populate(2, 3, 4.f);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3 });
		this->check<float>(floats, { 2.f, 4.f });
	}

	this->populate(3, 5, 6.f);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 5 });
		this->check<float>(floats, { 2.f, 4.f, 6.f });
	}

	this->populate(3, 7, 8.f);

}

using GetAllIntDouble = GetAllElementsTest<32, int, double>;
TEST_F(GetAllIntDouble, GetAllElements)
{

	this->init(2);

	this->populate(1, 1, 2.);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1 });
		this->check<double>(floats, { 2. });
	}

	this->populate(2, 3, 4.);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3 });
		this->check<double>(floats, { 2., 4. });
	}

	this->populate(2, 5, 6.f);

}


template <size_t memorySize, size_t srcMemorySize, typename ... Types>
class AppendElementsTest : public ::testing::Test {

protected:
	void init(int32_t expectCapacityEls, int32_t expectSrcCapacityEls)
	{

		MemorySpan memSpan(this->memory.data(), this->memory.data() + this->memory.size());
		Dod::DataUtils::initFromMemory(this->table, memSpan);
		ASSERT_EQ(this->table.capacityEls, expectCapacityEls);

		MemorySpan memSpanSrc(this->srcMemory.data(), this->srcMemory.data() + this->srcMemory.size());
		Dod::DataUtils::initFromMemory(this->srcTable, memSpanSrc);
		ASSERT_EQ(this->srcTable.capacityEls, expectSrcCapacityEls);

	}

	void populate(int32_t expectedNumOfFilledEls, Types&& ... values)
	{
		Dod::DataUtils::populate<Types...>(this->srcTable, true, std::forward<Types>(values)...);
		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->srcTable), expectedNumOfFilledEls);
	}

	template <typename T>
	void check(auto&& buffer, std::vector<T> expectedValues)
	{
		static_assert(std::is_same_v<std::decay_t<decltype(buffer)>, Dod::ImBuffer<T>>);

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(buffer), expectedValues.size());

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(buffer, id++), expected);
	}

	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

	std::array<Dod::MemTypes::data_t, srcMemorySize> srcMemory{};
	Dod::DBTable<Types...> srcTable;

};

using AppendIntFloat = AppendElementsTest<64, 32, int, float>;
TEST_F(AppendIntFloat, Append2Elements)
{

	this->init(7, 3);
	this->populate(1 ,1, 2.f);
	this->populate(2, 3, 4.f);

	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3 });
		this->check<float>(floats, { 2.f, 4.f });
	}
	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 1, 3 });
		this->check<float>(floats, { 2.f, 4.f, 2.f, 4.f });
	}

}

TEST_F(AppendIntFloat, Append3Elements)
{

	this->init(7, 3);
	this->populate(1, 1, 2.f);
	this->populate(2, 3, 4.f);
	this->populate(3, 5, 6.f);

	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 3);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 5 });
		this->check<float>(floats, { 2.f, 4.f, 6.f });
	}
	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 6);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 5, 1, 3, 5 });
		this->check<float>(floats, { 2.f, 4.f, 6.f, 2.f, 4.f, 6.f });
	}
	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 7);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 5, 1, 3, 5, 1 });
		this->check<float>(floats, { 2.f, 4.f, 6.f, 2.f, 4.f, 6.f, 2.f });
	}

}

TEST_F(AppendIntFloat, Append0Elements)
{

	this->init(7, 3);

	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(ints), 0);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(floats), 0);
	}

}

using AppendIntDouble = AppendElementsTest<64, 32, int, double>;
TEST_F(AppendIntDouble, Append3Elements)
{

	this->init(4, 2);
	this->populate(1, 1, 2.);
	this->populate(2, 3, 4.);

	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 2);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3 });
		this->check<double>(floats, { 2., 4. });
	}
	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 1, 3 });
		this->check<double>(floats, { 2., 4., 2., 4. });
	}
	Dod::DataUtils::append(this->table, this->srcTable);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 4);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1, 3, 1, 3 });
		this->check<double>(floats, { 2., 4., 2., 4. });
	}

}

template <size_t memorySize, typename ... Types>
class FlushAllElementsTest : public ::testing::Test {

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
	void check(auto&& buffer, std::vector<T> expectedValues)
	{
		static_assert(std::is_same_v<std::decay_t<decltype(buffer)>, Dod::ImBuffer<T>>);

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(buffer), expectedValues.size());

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(buffer, id++), expected);
	}


	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};

using FlushIntFloat = FlushAllElementsTest<32, int, float>;
TEST_F(FlushIntFloat, FlushAllElements)
{

	this->init(3);
	this->populate(1, 1, 2.f);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1 });
		this->check<float>(floats, { 2.f });
	}
	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);

	this->populate(1, 3, 4.f);
	this->populate(2, 5, 6.f);
	this->populate(3, 7, 8.f);
	this->populate(3, 9, 10.f);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 3, 5, 7 });
		this->check<float>(floats, { 4.f, 6.f, 8.f });
	}

	Dod::DataUtils::flush(this->table);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 0);
	this->populate(1, 11, 12.f);
	this->populate(2, 13, 14.f);
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 11, 13 });
		this->check<float>(floats, { 12.f, 14.f });
	}

}

template <size_t memorySize, typename ... Types>
class RemoveElementsTest : public ::testing::Test {

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
	void check(auto&& buffer, std::vector<T> expectedValues)
	{
		static_assert(std::is_same_v<std::decay_t<decltype(buffer)>, Dod::ImBuffer<T>>);

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(buffer), expectedValues.size());

		for (int32_t id{}; const auto expected : expectedValues)
			EXPECT_EQ(Dod::DataUtils::get(buffer, id++), expected);
	}


	std::array<Dod::MemTypes::data_t, memorySize> memory{};
	Dod::DBTable<Types...> table;

};


using RemoveElementsIntFloat = RemoveElementsTest<32, int, float>;
TEST_F(RemoveElementsIntFloat, RemoveElements)
{

	this->init(3);

	this->populate(1, 1, 2.f);
	this->populate(2, 3, 4.f);

	{
		std::array<int32_t, 2> indicesMem{ 0, 1 };
		Dod::ImBuffer<int32_t> indicesToRemove;
		Dod::DataUtils::initFromArray(indicesToRemove, indicesMem);
		Dod::DataUtils::remove(this->table, indicesToRemove);
		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(this->table), 1);
	}
	{
		const auto [ints, floats] { Dod::DataUtils::get(this->table) };
		this->check<int>(ints, { 1 });
		this->check<float>(floats, { 2.f });
	}

}