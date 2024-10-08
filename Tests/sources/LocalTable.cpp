#include <gtest/gtest.h>

#include <dod/MemPool.h>

#include <engine/LocalTable.h>

namespace
{
}

TEST(LocalTable, creation)
{

	Engine::LocalTable::MemoryPool mem(256);
	Dod::MemTypes::capacity_t header{};

	auto table1{ Engine::LocalTable::create<int>(16, mem, header) };
	
	EXPECT_EQ(Dod::DataUtils::getCapacity(table1), 16);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table1), 0);
	EXPECT_NE(header, 0);

	Dod::DataUtils::pushBack(table1, true, 1);
	EXPECT_EQ(Dod::DataUtils::get(table1, 0), 1);


	auto table2{ Engine::LocalTable::create<int>(16, mem, header) };

	EXPECT_EQ(Dod::DataUtils::getCapacity(table2), 16);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table2), 0);
	EXPECT_NE(header, 0);

	Dod::DataUtils::pushBack(table2, true, 2);
	EXPECT_EQ(Dod::DataUtils::get(table1, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(table2, 0), 2);

}

TEST(LocalTable, creationReset)
{

	Engine::LocalTable::MemoryPool mem(256);
	Dod::MemTypes::capacity_t header{};

	auto table1{ Engine::LocalTable::create<int>(16, mem) };

	EXPECT_EQ(Dod::DataUtils::getCapacity(table1), 16);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table1), 0);

	Dod::DataUtils::pushBack(table1, true, 1);
	EXPECT_EQ(Dod::DataUtils::get(table1, 0), 1);

	auto table2{ Engine::LocalTable::create<int>(16, mem) };

	EXPECT_EQ(Dod::DataUtils::getCapacity(table2), 16);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table2), 0);

	Dod::DataUtils::pushBack(table2, true, 2);
	EXPECT_EQ(Dod::DataUtils::get(table1, 0), 2);
	EXPECT_EQ(Dod::DataUtils::get(table2, 0), 2);

}

TEST(LocalTable, creationResetStableHeader)
{

	Engine::LocalTable::MemoryPool mem(256);
	Dod::MemTypes::capacity_t header{};

	auto table1{ Engine::LocalTable::create<int>(16, mem, header) };

	EXPECT_EQ(Dod::DataUtils::getCapacity(table1), 16);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table1), 0);
	EXPECT_NE(header, 0);

	Dod::DataUtils::pushBack(table1, true, 1);
	EXPECT_EQ(Dod::DataUtils::get(table1, 0), 1);

	const auto stage1Header{ header };
	auto table2{ Engine::LocalTable::create<int>(16, mem, stage1Header) };

	EXPECT_EQ(Dod::DataUtils::getCapacity(table2), 16);
	EXPECT_EQ(Dod::DataUtils::getNumFilledElements(table2), 0);
	EXPECT_EQ(header, stage1Header);
	
	Dod::DataUtils::pushBack(table2, true, 2);
	EXPECT_EQ(Dod::DataUtils::get(table1, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(table2, 0), 2);

}
