#include <gtest/gtest.h>

#include <dod/MemPool.h>

#include <dod/Tables.h>
#include <engine/TablesSequence.h>

#include <array>

TEST(TableSequence, twoTables)
{

	int point1{};
	int point2{};

	Dod::ImTable<int32_t> table1{};
	table1.dataBegin = reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(&point1);
	table1.numOfElements = 42;

	Dod::ImTable<int32_t> table2{};
	table2.dataBegin = reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(&point2);
	table2.numOfElements = 123;

	const auto seq{ Engine::TableSequence::Sequence(Dod::ImTable(table1), Dod::ImTable(table2)) };
	
	const auto expected{ std::to_array<Dod::ImTable<int32_t>>({
		Dod::ImTable(table1),
		Dod::ImTable(table2),
	}) };

	size_t currentIndex{};
	
	Engine::TableSequence::forEeach(seq, [&](auto&& table) {
		ASSERT_EQ(table.dataBegin, expected[currentIndex].dataBegin);
		ASSERT_EQ(table.numOfElements, expected[currentIndex].numOfElements);
		static_assert(std::is_same_v<decltype(table.dataBegin), Dod::MemTypes::dataConstPoint_t>);

		++currentIndex;
	});

	EXPECT_EQ(currentIndex, expected.size());

	EXPECT_EQ(Engine::TableSequence::getNumOfElements(seq), 165);

}

TEST(TableSequence, twoTablesMultitypes)
{

	int point1{};
	int point2{};

	Dod::ImTable<int32_t, float> table1{};
	table1.dataBegin = reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(&point1);
	table1.numOfElements = 42;

	Dod::ImTable<int32_t, float> table2{};
	table2.dataBegin = reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(&point2);
	table2.numOfElements = 16;

	const auto seq{ Engine::TableSequence::Sequence(Dod::ImTable(table1), Dod::ImTable(table2)) };

	const auto expected{ std::to_array<Dod::ImTable<int32_t, float>>({
		Dod::ImTable(table1),
		Dod::ImTable(table2),
	}) };

	size_t currentIndex{};

	Engine::TableSequence::forEeach(seq, [&](auto&& table) {
		ASSERT_EQ(table.dataBegin, expected[currentIndex].dataBegin);
		ASSERT_EQ(table.numOfElements, expected[currentIndex].numOfElements);
		static_assert(std::is_same_v<decltype(table.dataBegin), Dod::MemTypes::dataConstPoint_t>);

		++currentIndex;
	});

	EXPECT_EQ(currentIndex, expected.size());

	EXPECT_EQ(Engine::TableSequence::getNumOfElements(seq), 58);

}

struct Table1 : public Dod::ImTable<int32_t> {};
struct Table2 : public Dod::ImTable<int32_t> {};
struct Table3 : public Dod::ImTable<int32_t> {};

TEST(TableSequence, twoTableWrappers)
{

	Table1 table1{};
	Table2 table2{};
	Table3 table3{};

	using seq_t = Engine::TableSequence::Sequence<Table1, Table2, Table3>;
	const auto seq{ seq_t(table1, table2, table3) };

	const auto expected{ std::to_array<Dod::ImTable<int32_t>>({
		Dod::ImTable(table1),
		Dod::ImTable(table2),
		Dod::ImTable(table3),
	}) };

	size_t currentIndex{};

	Engine::TableSequence::forEeach(seq, [&](auto&& table) {
		ASSERT_EQ(table.dataBegin, expected[currentIndex].dataBegin);
		ASSERT_EQ(table.numOfElements, expected[currentIndex].numOfElements);
		static_assert(std::is_same_v<decltype(table.dataBegin), Dod::MemTypes::dataConstPoint_t>);

		++currentIndex;
	});

	EXPECT_EQ(currentIndex, expected.size());

	EXPECT_EQ(Engine::TableSequence::getNumOfElements(seq), 0);

}

TEST(TableSequence, twoTableCopy)
{

	int point1{};
	int point2{};

	Dod::ImTable<int32_t> table1{};
	table1.dataBegin = reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(&point1);
	table1.numOfElements = 42;

	Dod::ImTable<int32_t> table2{};
	table2.dataBegin = reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(&point2);
	table2.numOfElements = 123;

	using seq_t = Engine::TableSequence::Sequence<Dod::ImTable<int32_t>, Dod::ImTable<int32_t>>;
	const auto seq{ seq_t(table1, table2) };

	const auto expected{ std::to_array<Dod::ImTable<int32_t>>({
		Dod::ImTable(table1),
		Dod::ImTable(table2),
	}) };

	size_t currentIndex{};

	const auto seqCopied{ seq };

	Engine::TableSequence::forEeach(seqCopied, [&](auto&& table) {
		ASSERT_EQ(table.dataBegin, expected[currentIndex].dataBegin);
		ASSERT_EQ(table.numOfElements, expected[currentIndex].numOfElements);
		static_assert(std::is_same_v<decltype(table.dataBegin), Dod::MemTypes::dataConstPoint_t>);

		++currentIndex;
	});

	EXPECT_EQ(currentIndex, expected.size());

	EXPECT_EQ(Engine::TableSequence::getNumOfElements(seqCopied), 165);

}

TEST(TableSequence, twoTablesMutable)
{

	int point1{};
	int point2{};

	Dod::MutTable<int32_t> table1{};
	table1.dataBegin = reinterpret_cast<Dod::MemTypes::dataPoint_t>(&point1);
	table1.numOfElements = 42;

	Dod::MutTable<int32_t> table2{};
	table2.dataBegin = reinterpret_cast<Dod::MemTypes::dataPoint_t>(&point2);
	table2.numOfElements = 123;

	const auto seq{ Engine::TableSequence::Sequence(Dod::MutTable(table1), Dod::MutTable(table2)) };

	const auto expected{ std::to_array<Dod::ImTable<int32_t>>({
		Dod::ImTable(table1),
		Dod::ImTable(table2),
	}) };

	size_t currentIndex{};

	Engine::TableSequence::forEeach(seq, [&](auto&& table) {
		ASSERT_EQ(table.dataBegin, expected[currentIndex].dataBegin);
		ASSERT_EQ(table.numOfElements, expected[currentIndex].numOfElements);
		static_assert(std::is_same_v<decltype(table.dataBegin), Dod::MemTypes::dataPoint_t>);
		++currentIndex;
	});

	EXPECT_EQ(currentIndex, expected.size());

	EXPECT_EQ(Engine::TableSequence::getNumOfElements(seq), 165);

}
