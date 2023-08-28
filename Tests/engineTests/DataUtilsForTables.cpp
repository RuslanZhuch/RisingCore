#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/DataUtils.h>
#include <dod/Tables.h>

#include "utils/CommonStructures.h"

#pragma warning(push)
#pragma warning(disable : 4365)

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

	std::array<Dod::MemTypes::data_t, memorySize> memory;
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

TEST(TableUtils, PopulateIntFloat)
{

	constexpr auto dataSize{ 32 };
	std::array<Dod::MemTypes::data_t, dataSize> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBTable<int, float> table;

	Dod::DataUtils::initFromMemory(table, memSpan);

	Dod::DataUtils::populate(table, true, 1, 2.f);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 1);

	const auto deadBucketSize{ 4 };
	
	{
		const auto populatedIntPtr{ reinterpret_cast<int32_t*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedIntPtr, 1);

		const auto floatCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(int) * table.capacityEls };
		const auto populatedFloatPtr{ (float*)floatCollumnMemOffset };
		EXPECT_EQ(*populatedFloatPtr, 2.f);
	}

	Dod::DataUtils::populate(table, true, 3, 4.f);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 2);

	{
		const auto populatedIntPtr{ reinterpret_cast<int32_t*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedIntPtr, 1);
		EXPECT_EQ(*populatedIntPtr, 1);
		EXPECT_EQ(*(populatedIntPtr + 1), 3);

		const auto floatCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(int) * table.capacityEls };
		const auto populatedFloatPtr{ (float*)floatCollumnMemOffset };
		EXPECT_EQ(*populatedFloatPtr, 2.f);
		EXPECT_EQ(*(populatedFloatPtr + 1), 4.f);
	}

	Dod::DataUtils::populate(table, true, 5, 6.f);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 3);
	{
		const auto populatedIntPtr{ reinterpret_cast<int32_t*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedIntPtr, 1);
		EXPECT_EQ(*(populatedIntPtr + 1), 3);
		EXPECT_EQ(*(populatedIntPtr + 2), 5);

		const auto floatCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(int) * table.capacityEls };
		const auto populatedFloatPtr{ (float*)floatCollumnMemOffset };
		EXPECT_EQ(*populatedFloatPtr, 2.f);
		EXPECT_EQ(*(populatedFloatPtr + 1), 4.f);
		EXPECT_EQ(*(populatedFloatPtr + 2), 6.f);
	}
	Dod::DataUtils::populate(table, true, 7, 8.f);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table), 3);

//	Dod::ImBuffer<int> col1{ Dod::DataUtils::get<0>(table) };
//	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(col1), 1);
//	EXPECT_EQ(Dod::DataUtils::get(col1, 0), 1);
//
//	Dod::ImBuffer<float> col2{ Dod::DataUtils::get<1>(table) };
//	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(col2), 1);
//	EXPECT_EQ(Dod::DataUtils::get(col2, 0), 2.f);

}

TEST(TableUtils, PopulateIntDouble)
{

	constexpr auto dataSize{ 32 };
	std::array<Dod::MemTypes::data_t, dataSize> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBTable<int, double> table;

	Dod::DataUtils::initFromMemory(table, memSpan);

	Dod::DataUtils::populate(table, true, 1, 2.);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 1);

	const auto deadBucketSize{ 8 };

	{
		const auto populatedIntPtr{ reinterpret_cast<int32_t*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedIntPtr, 1);

		const auto doubleCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(int) * table.capacityEls };
		const auto populatedDoublePtr{ (double*)doubleCollumnMemOffset };
		EXPECT_EQ(*populatedDoublePtr, 2.);
	}

	Dod::DataUtils::populate(table, true, 3, 4.);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 2);

	{
		const auto populatedIntPtr{ reinterpret_cast<int32_t*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedIntPtr, 1);
		EXPECT_EQ(*populatedIntPtr, 1);
		EXPECT_EQ(*(populatedIntPtr + 1), 3);

		const auto doubleCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(int) * table.capacityEls };
		const auto populatedDoublePtr{ (double*)doubleCollumnMemOffset };
		EXPECT_EQ(*populatedDoublePtr, 2.f);
		EXPECT_EQ(*(populatedDoublePtr + 1), 4.f);
	}

	Dod::DataUtils::populate(table, true, 5, 6.);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 2);

}

TEST(TableUtils, PopulateDoubleInt)
{

	constexpr auto dataSize{ 32 };
	std::array<Dod::MemTypes::data_t, dataSize> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBTable<double, int> table;

	Dod::DataUtils::initFromMemory(table, memSpan);

	Dod::DataUtils::populate(table, true, 1., 2);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 1);

	const auto deadBucketSize{ 8 };

	{
		const auto populatedDoublePtr{ reinterpret_cast<double*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedDoublePtr, 1.);

		const auto intCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(double) * table.capacityEls };
		const auto populatedIntPtr{ (int*)intCollumnMemOffset };
		EXPECT_EQ(*populatedIntPtr, 2);
	}

	Dod::DataUtils::populate(table, true, 3., 4);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 2);

	{
		const auto populatedDoublePtr{ reinterpret_cast<double*>(table.dataBegin + deadBucketSize) };
		EXPECT_EQ(*populatedDoublePtr, 1);
		EXPECT_EQ(*populatedDoublePtr, 1);
		EXPECT_EQ(*(populatedDoublePtr + 1), 3);

		const auto intCollumnMemOffset{ table.dataBegin + deadBucketSize + sizeof(double) * table.capacityEls };
		const auto populatedIntPtr{ (int*)intCollumnMemOffset };
		EXPECT_EQ(*populatedIntPtr, 2.f);
		EXPECT_EQ(*(populatedIntPtr + 1), 4.f);
	}

	Dod::DataUtils::populate(table, true, 5., 6);
	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(table), 2);

}
