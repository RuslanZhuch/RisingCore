#include "pch.h"

#include <dod/Buffers.h>
#include <dod/DataUtils.h>
#include <dod/Algorithms.h>

#include <dod/Tables.h>
#include <dod/TableUtils.h>


#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

template <typename T>
static void initDBuffer(Dod::DBBuffer<T>& dest, auto& src)
{

	struct MemorySpan
	{
		Dod::MemTypes::dataPoint_t dataBegin{};
		Dod::MemTypes::dataPoint_t dataEnd{};
	};
	MemorySpan memSpan(reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()), Dod::MemTypes::dataPoint_t(src.data() + src.size()));
	Dod::DataUtils::initFromMemory(dest, memSpan);
	dest.numOfFilledEls = static_cast<int32_t>(src.size()) - 1;

}

template <typename T>
static void initDTable(Dod::DTable<T>& dest, auto& src)
{

	struct MemorySpan
	{
		Dod::MemTypes::dataPoint_t dataBegin{};
		Dod::MemTypes::dataPoint_t dataEnd{};
	};
	MemorySpan memSpan(reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()), Dod::MemTypes::dataPoint_t(src.data()) + 64);
	Dod::DataUtils::initFromMemory(dest, src.size(), memSpan);
	dest.numOfFilledEls = static_cast<int32_t>(src.size());

}

template <typename T>
static void initDTableFromArray(Dod::DTable<T>& dest, auto& src)
{

	struct MemorySpan
	{
		Dod::MemTypes::dataPoint_t dataBegin{};
		Dod::MemTypes::dataPoint_t dataEnd{};
	};
	MemorySpan memSpan(reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()), Dod::MemTypes::dataPoint_t(src.data()) + 64);
	Dod::DataUtils::initFromMemory(dest, src.size(), memSpan);

}

TEST(Algorithms, LeftUniques)
{

	using type_t = int32_t;

	{
		alignas(64) auto values{ std::to_array<type_t>({1, 1, 2, 2, 3, 3, 4, 4}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 4);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), 2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), 3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 3), 4);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({1, 1, 1, 1, 1, 1}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 1);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({1, 2, 2, 2, 2, 2}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), 2);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({2, 2, 2, 2, 2, 1}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), 1);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({1, 2, 2, 2, 2, 3}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 3);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 1), 2);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 2), 3);
	}

	{
		Dod::DTable<type_t> buffer;

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 0);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({1}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(buffer), 1);
		EXPECT_EQ(Dod::DataUtils::get(buffer, 0), 1);
	}

}

TEST(Algorithm, GetSortedUniques)
{
	using type_t = int32_t;

	{
		alignas(64) auto values{ std::to_array<type_t>({2, 1, 1, 2, 3, 4, 3, 5, 4}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		alignas(64) auto results{ std::array<int32_t, values.size()>() };
		Dod::DTable<int32_t> resultbuffer;
		initDTableFromArray(resultbuffer, results);

		Dod::Algorithms::getSortedUniqueElsIndices(resultbuffer, Dod::ImTable(buffer));

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 5);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 0);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 2), 4);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 3), 5);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 4), 7);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({5, 5, 4, 3, 3, 2}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		alignas(64) auto results{ std::array<int32_t, values.size()>() };
		Dod::DTable<int32_t> resultbuffer;
		initDTableFromArray(resultbuffer, results);

		Dod::Algorithms::getSortedUniqueElsIndices(resultbuffer, Dod::ImTable(buffer));

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 4);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 5);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 3);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 2), 2);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 3), 0);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({2, 3, 3, 4, 5, 5}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		alignas(64) auto results{ std::array<int32_t, values.size()>() };
		Dod::DTable<int32_t> resultbuffer;
		initDTableFromArray(resultbuffer, results);

		Dod::Algorithms::getSortedUniqueElsIndices(resultbuffer, Dod::ImTable(buffer));

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 4);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 0);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 2), 3);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 3), 4);
	}

	{
		alignas(64) auto values{ std::to_array<type_t>({2, 2, 2, 2, 2, 2}) };

		Dod::DTable<type_t> buffer;
		initDTable(buffer, values);

		alignas(64) auto results{ std::array<int32_t, values.size()>() };
		Dod::DTable<int32_t> resultbuffer;
		initDTableFromArray(resultbuffer, results);

		Dod::Algorithms::getSortedUniqueElsIndices(resultbuffer, Dod::ImTable(buffer));

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 0);
	}

	{
		Dod::DTable<type_t> buffer;

		Dod::DTable<int32_t> resultbuffer;

		Dod::Algorithms::getSortedUniqueElsIndices(resultbuffer, Dod::ImTable(buffer));

		ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 0);
	}

}

TEST(Algorithm, GetIndicesByValue)
{
	using type_t = int32_t;

	{
		auto values{ std::to_array<type_t>({0, 2, 1, 1, 2, 3, 4, 3, 5, 4}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		auto results{ std::array<int32_t, values.size()>() };
		Dod::DBBuffer<int32_t> resultbuffer;
		Dod::DataUtils::initFromArray(resultbuffer, results);

		{
			Dod::Algorithms::getIndicesByValue(resultbuffer, Dod::DataUtils::createImFromBuffer(buffer), 1);

			ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 2);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 1);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 2);
		}
		{
			Dod::Algorithms::getIndicesByValue(resultbuffer, Dod::DataUtils::createImFromBuffer(buffer), 2);

			ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 2);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 0);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 3);
		}
		{
			Dod::Algorithms::getIndicesByValue(resultbuffer, Dod::DataUtils::createImFromBuffer(buffer), 3);

			ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 2);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 4);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 6);
		}
		{
			Dod::Algorithms::getIndicesByValue(resultbuffer, Dod::DataUtils::createImFromBuffer(buffer), 4);

			ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 2);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 5);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 1), 8);
		}
		{
			Dod::Algorithms::getIndicesByValue(resultbuffer, Dod::DataUtils::createImFromBuffer(buffer), 5);

			ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 1);
			EXPECT_EQ(Dod::DataUtils::get(resultbuffer, 0), 7);
		}
		{
			Dod::Algorithms::getIndicesByValue(resultbuffer, Dod::DataUtils::createImFromBuffer(buffer), 10);

			ASSERT_EQ(Dod::DataUtils::getNumFilledElements(resultbuffer), 0);
		}
	}
}

TEST(Algorithms, GetIntersections)
{

	using type_t = int32_t;
	struct Buffer
	{
		const type_t* dataBegin{};
		const type_t* dataEnd{};
	};

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 3);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 1), 2);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 2), 3);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 1, 2 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 2);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 1), 2);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 1, 2 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 2);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 1), 2);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 1, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 1, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 4);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 1), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 2), 2);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 3), 3);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 1, 1, 2, 3 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 4, 5, 6, 7 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 0);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 4, 5, 6, 7 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 0);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 4, 5, 6, 7 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 0);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 0);
	}

	{
		auto valuesLeft{ std::to_array<type_t>({ 0, 1 }) };
		Dod::DBBuffer<type_t> bufferLeft;
		initDBuffer(bufferLeft, valuesLeft);

		auto valuesRight{ std::to_array<type_t>({ 0, 1 }) };
		Dod::DBBuffer<type_t> bufferRight;
		initDBuffer(bufferRight, valuesRight);

		std::array<type_t, std::max(valuesLeft.size(), valuesRight.size())> resultMemory;
		Dod::DBBuffer<type_t> resultBuffer;
		Dod::DataUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::DataUtils::getNumFilledElements(resultBuffer), 1);
		EXPECT_EQ(Dod::DataUtils::get(resultBuffer, 0), 1);
	}

}

TEST(Algorithms, GetSortedIndices)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({10, 1, 5, -5, 6, 4}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> indices;
	Dod::DTable<int32_t> indicesBuffer;
	initDTableFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), Dod::DataUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 1), 1);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 2), 5);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 3), 2);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 4), 4);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 5), 0);

}

TEST(Algorithms, GetSortedIndicesRepeats)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({10, 1, 1, -5, 1, 4}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> indices;
	Dod::DTable<int32_t> indicesBuffer;
	initDTableFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), Dod::DataUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 1), 1);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 2), 2);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 3), 4);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 4), 5);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 5), 0);

}

TEST(Algorithms, GetSortedIndicesOneEl)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({10}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> indices;
	Dod::DTable<int32_t> indicesBuffer;
	initDTableFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), Dod::DataUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 0), 0);

}

TEST(Algorithms, GetSortedIndicesNoEls)
{

	using type_t = int32_t;

	Dod::DTable<type_t> buffer;

	Dod::DTable<int32_t> indicesBuffer;

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), Dod::DataUtils::getNumFilledElements(buffer));

}

TEST(Algorithms, GetSortedIndicesNoSpace)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({10, 1, 5, -5, 6, 4}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, 3> indices;
	Dod::DTable<int32_t> indicesBuffer;
	initDTableFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), 3);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 1), 2);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 2), 0);

}

TEST(Algorithms, GetSortedIndicesEmptyTarget)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({10, 1, 5, -5, 6, 4}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	Dod::DTable<int32_t> indicesBuffer;

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), 0);

}

TEST(Algorithms, GetSortedIndicesLargeTarget)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({10, 1, 5, -5, 6, 4}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, 14> indices;
	indices.fill(1);
	Dod::DTable<int32_t> indicesBuffer;
	initDTableFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), Dod::DataUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 1), 1);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 2), 5);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 3), 2);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 4), 4);
	EXPECT_EQ(Dod::DataUtils::get(indicesBuffer, 5), 0);

}

TEST(Algorithms, GetSortedIndicesEmptySrc)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	std::array<int32_t, 6> indices;
	Dod::DTable<int32_t> indicesBuffer;
	initDTableFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(indicesBuffer), 0);

}

TEST(Algorithms, CountUniques)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({0, 0, 1, 2, 2, 5, 7, 7, 7}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 1), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 2), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 3), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 4), 3);

}

TEST(Algorithms, CountUniquesOneElsOnSides)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({0, 1, 2, 2, 5, 7}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 1), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 2), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 3), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 4), 1);

}

TEST(Algorithms, CountUniquesOnlyMultiples)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({0, 0, 1, 1, 1, 2, 2, 5, 5, 7, 7, 7}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 1), 3);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 2), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 3), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 4), 3);

}

TEST(Algorithms, CountUniquesOnlySingles)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({0, 1, 2, 5, 7}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 1), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 2), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 3), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 4), 1);

}

TEST(Algorithms, CountUniquesOneMultiple)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({1, 1, 1}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 3);

}

TEST(Algorithms, CountUniquesOneSingle)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({1}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, values.size()> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 1);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 1);

}

TEST(Algorithms, CountUniquesEmpty)
{

	using type_t = int32_t;

	Dod::DTable<type_t> buffer;

	Dod::DTable<int32_t> countersBuffer;

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 0);

}

TEST(Algorithms, CountUniquesNotEnouthCounters)
{

	using type_t = int32_t;

	alignas(64) auto values{ std::to_array<type_t>({0, 0, 1, 2, 2, 5, 7, 7, 7}) };

	Dod::DTable<type_t> buffer;
	initDTable(buffer, values);

	alignas(64) std::array<int32_t, 2> counters;
	Dod::DTable<int32_t> countersBuffer;
	initDTableFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::ImTable(buffer)
	);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(countersBuffer), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 0), 2);
	EXPECT_EQ(Dod::DataUtils::get(countersBuffer, 1), 1);

}
