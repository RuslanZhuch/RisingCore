#include "pch.h"

#include <dod/Buffers.h>
#include <dod/BufferUtils.h>
#include <dod/Algorithms.h>


#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

template <typename T>
static void initDBuffer(Dod::DBBuffer<T>& dest, auto& src)
{

	const size_t totalElements{ src.size() };
	const auto totalBytes{ static_cast<int32_t>(totalElements * sizeof(T)) };

	std::memcpy(src.data(), src.data(), src.size());

	const Dod::MemTypes::capacity_t beginIndex{ 0 };
	const Dod::MemTypes::capacity_t endIndex{ totalBytes };

	struct MemorySpan
	{
		Dod::MemTypes::dataPoint_t dataBegin{};
		Dod::MemTypes::dataPoint_t dataEnd{};
	};
	MemorySpan memSpan(reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()), Dod::MemTypes::dataPoint_t(src.data() + src.size()));
	Dod::BufferUtils::initFromMemory(dest, memSpan, beginIndex, endIndex);
	dest.numOfFilledEls = static_cast<int32_t>(totalElements) - 1;

}

TEST(Algorithms, LeftUniques)
{

	using type_t = int32_t;

	{
		auto values{ std::to_array<type_t>({0, 1, 1, 2, 2, 3, 3, 4, 4}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 4);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), 3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), 4);
	}

	{
		auto values{ std::to_array<type_t>({0, 1, 1, 1, 1, 1, 1}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
	}

	{
		auto values{ std::to_array<type_t>({0, 1, 2, 2, 2, 2, 2}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 2);
	}

	{
		auto values{ std::to_array<type_t>({0, 2, 2, 2, 2, 2, 1}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 1);
	}

	{
		auto values{ std::to_array<type_t>({0, 1, 2, 2, 2, 2, 3}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), 3);
	}

	{
		auto values{ std::to_array<type_t>({0}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	}

	{
		auto values{ std::to_array<type_t>({0, 1}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 3);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 1), 2);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 2), 3);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 2);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 1), 2);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 2);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 1), 2);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 4);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 1), 1);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 2), 2);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 3), 3);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 0);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 0);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 0);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 0);
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
		Dod::BufferUtils::initFromArray(resultBuffer, resultMemory);

		Dod::Algorithms::getIntersections(resultBuffer, bufferLeft, bufferRight);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(resultBuffer), 1);
		EXPECT_EQ(Dod::BufferUtils::get(resultBuffer, 0), 1);
	}

}

TEST(Algorithms, GetSortedIndices)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 10, 1, 5, -5, 6, 4}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), Dod::BufferUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 0), 3);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 2), 5);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 3), 2);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 4), 4);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 5), 0);

}

TEST(Algorithms, GetSortedIndicesRepeats)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 10, 1, 1, -5, 1, 4}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), Dod::BufferUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 0), 3);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 2), 2);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 3), 4);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 4), 5);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 5), 0);

}

TEST(Algorithms, GetSortedIndicesOneEl)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 10}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), Dod::BufferUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 0), 0);

}

TEST(Algorithms, GetSortedIndicesNoEls)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), Dod::BufferUtils::getNumFilledElements(buffer));

}

TEST(Algorithms, GetSortedIndicesNoSpace)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 10, 1, 5, -5, 6, 4}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, 4> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), 3);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 2), 0);

}

TEST(Algorithms, GetSortedIndicesEmptyTarget)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 10, 1, 5, -5, 6, 4}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, 1> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), 0);

}

TEST(Algorithms, GetSortedIndicesLargeTarget)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 10, 1, 5, -5, 6, 4}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, 15> indices;
	indices.fill(1);
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), Dod::BufferUtils::getNumFilledElements(buffer));
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 0), 3);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 2), 5);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 3), 2);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 4), 4);
	EXPECT_EQ(Dod::BufferUtils::get(indicesBuffer, 5), 0);

}

TEST(Algorithms, GetSortedIndicesEmptySrc)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, 6> indices;
	Dod::DBBuffer<int32_t> indicesBuffer;
	Dod::BufferUtils::initFromArray(indicesBuffer, indices);

	Dod::Algorithms::getSortedIndices(
		indicesBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(indicesBuffer), 0);

}

TEST(Algorithms, CountUniques)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 0, 0, 1, 2, 2, 5, 7, 7, 7}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 2), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 3), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 4), 3);

}

TEST(Algorithms, CountUniquesOneElsOnSides)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 0, 1, 2, 2, 5, 7}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 2), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 3), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 4), 1);

}

TEST(Algorithms, CountUniquesOnlyMultiples)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 0, 0, 1, 1, 1, 2, 2, 5, 5, 7, 7, 7}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 1), 3);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 2), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 3), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 4), 3);

}

TEST(Algorithms, CountUniquesOnlySingles)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 0, 1, 2, 5, 7}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 5);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 2), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 3), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 4), 1);

}

TEST(Algorithms, CountUniquesOneMultiple)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 1, 1, 1}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 3);

}

TEST(Algorithms, CountUniquesOneSingle)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 1}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 1);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 1);

}

TEST(Algorithms, CountUniquesEmpty)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, values.size()> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 0);

}

TEST(Algorithms, CountUniquesNotEnouthCounters)
{

	using type_t = int32_t;

	auto values{ std::to_array<type_t>({0, 0, 0, 1, 2, 2, 5, 7, 7, 7}) };

	Dod::DBBuffer<type_t> buffer;
	initDBuffer(buffer, values);

	std::array<int32_t, 3> counters;
	Dod::DBBuffer<int32_t> countersBuffer;
	Dod::BufferUtils::initFromArray(countersBuffer, counters);

	Dod::Algorithms::countUniques(
		countersBuffer,
		Dod::BufferUtils::createImFromBuffer(buffer)
	);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(countersBuffer), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(countersBuffer, 1), 1);

}
