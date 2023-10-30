#include "pch.h"

#include <dod/Tables.h>
#include <dod/DataUtils.h>

#include <dod/MemPool.h>
#include <dod/MemUtils.h>

#include <dod/DataStream.h>

#include <array>

template <typename TDest>
class StreamTest : public ::testing::Test {
protected:
	void init(Dod::MemTypes::capacity_t byteToAllocate)
	{
		this->memory.allocate(byteToAllocate + 64);
		Dod::DataUtils::initFromMemory(this->dest, byteToAllocate / sizeof(TDest), Dod::MemUtils::acquire(this->memory, 0, byteToAllocate, 64));
		this->header = 0;
		EXPECT_EQ(Dod::DataUtils::getCapacity(this->dest), byteToAllocate / sizeof(TDest));
	}

	template <typename TData>
	void serialize(TData data, Dod::MemTypes::capacity_t expectedHeaderOffset, Dod::MemTypes::capacity_t expectedPages)
	{
		const auto prevHeader{ this->header };
		this->header = Dod::DataStream::serialize(this->dest, data, this->header);

		const auto headerOffset{ this->header - prevHeader };
		EXPECT_EQ(headerOffset, expectedHeaderOffset);
//		EXPECT_EQ(this->dest.numOfFilledEls, expectedPages);
		if (headerOffset != 0)
			EXPECT_EQ(*reinterpret_cast<TData*>(dest.dataBegin + prevHeader), data);
	}

	template <typename TData>
	void serializeMultiple(std::vector<TData> data, Dod::MemTypes::capacity_t expectedHeaderOffset, Dod::MemTypes::capacity_t expectedPages)
	{
		const auto prevHeader{ this->header };
		this->header = Dod::DataStream::serialize(this->dest, data.data(), data.size(), this->header);

		const auto headerOffset{ this->header - prevHeader };
		EXPECT_EQ(headerOffset, expectedHeaderOffset);
//		EXPECT_EQ(this->dest.numOfFilledEls, expectedPages);
		if (headerOffset != 0)
		{
			EXPECT_EQ(*reinterpret_cast<int32_t*>(this->dest.dataBegin + prevHeader), data.size());
			EXPECT_EQ(std::memcmp(reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(data.data()), 
				this->dest.dataBegin + prevHeader + sizeof(int32_t), data.size() * sizeof(TData)), 0);
		}
	}

	template <typename TData>
	void deserialize(Dod::MemTypes::capacity_t expectedHeaderOffset, TData expectedData)
	{
		TData data{};
		const auto prevHeader{ this->header };
		this->header = Dod::DataStream::deserialize(this->dest, this->header, data);

		const auto headerOffset{ this->header - prevHeader };
		EXPECT_EQ(headerOffset, expectedHeaderOffset);
		//		EXPECT_EQ(this->dest.numOfFilledEls, expectedPages);
		if (headerOffset != 0)
			EXPECT_EQ(data, expectedData);
	}

	template <typename TData>
	void deserializeMultiple(Dod::MemTypes::capacity_t expectedHeaderOffset, std::vector<TData> expectedData)
	{
		TData* data{};
		int32_t numOfElements{};
		const auto prevHeader{ this->header };
		this->header = Dod::DataStream::deserialize(this->dest, this->header, numOfElements, data);

		const auto headerOffset{ this->header - prevHeader };
		EXPECT_EQ(headerOffset, expectedHeaderOffset);
		//		EXPECT_EQ(this->dest.numOfFilledEls, expectedPages);
		if (headerOffset != 0)
		{
			EXPECT_EQ(numOfElements, expectedData.size());
			EXPECT_EQ(std::memcmp(data, expectedData.data(), expectedData.size()), 0);
		}
	}

	Dod::MemPool memory;
	Dod::MemTypes::capacity_t header{};
	Dod::DTable<TDest> dest;

};

using serializeToInt32 = StreamTest<int32_t>;

TEST_F(serializeToInt32, Serialize)
{

	this->init(64);

	for (int32_t id{0}; id < 16; ++id)
		this->serialize(id + 1, 4, id + 1);

	this->serialize(17, 0, 16);

	this->init(64);

	for (int32_t id{ 0 }; id < 8; ++id)
		this->serialize(int64_t{ id + 1 }, 8, (id + 1) * 2);

	this->serialize(9, 0, 16);

	this->init(64);

	for (int32_t id{ 0 }; id < 8; ++id)
		this->serialize(id + 1, 4, id + 1);

	for (int32_t id{ 0 }; id < 4; ++id)
		this->serialize(int64_t{ id + 1 + 8 }, 8, 8 + (id + 1) * 2);

	this->init(64);

	for (int32_t id{ 0 }; id < 7; ++id)
		this->serialize(id + 1, 4, id + 1);

	for (int32_t id{ 0 }; id < 4; ++id)
		this->serialize(int64_t{ id + 1 + 7 }, 8, 7 + (id + 1) * 2);

}

using serializeToInt64 = StreamTest<int64_t>;

TEST_F(serializeToInt64, Serialize)
{

	this->init(64);

	for (int32_t id{ 0 }; id < 16; ++id)
		this->serialize(id + 1, 4, (id + 1) / 2);

	this->serialize(17, 0, 8);

	this->init(64);

	for (int32_t id{ 0 }; id < 8; ++id)
		this->serialize(int64_t{ id + 1 }, 8, id + 1);

	this->serialize(9, 0, 8);

	this->init(64);

	for (int32_t id{ 0 }; id < 8; ++id)
		this->serialize(id + 1, 4, (id + 1) / 2);

	for (int32_t id{ 0 }; id < 4; ++id)
		this->serialize(int64_t{ id + 1 + 8 }, 8, 4 + id + 1);

	this->init(64);

	const auto pages = std::to_array<int32_t>({
		1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8
	});

	for (int32_t id{ 0 }; id < 7; ++id)
		this->serialize(id + 1, 4, pages[id]);

	for (int32_t id{ 0 }; id < 4; ++id)
		this->serialize(int64_t{ id + 1 + 7 }, 8, pages[7 + id]);

}

TEST_F(serializeToInt32, SerializeMultiple)
{

	this->init(64);

	this->serializeMultiple<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8 }, 36, 9);
	this->serializeMultiple<int64_t>({ 9, 10, 11 }, 28, 16);

	this->init(64);
	this->serializeMultiple<int32_t>({ 12, 13, 14, 15, 16 }, 24, 6);
	this->serializeMultiple<int64_t>({ 17, 18, 19, 20 }, 36, 15);

	this->serializeMultiple<int32_t>({ 21 }, 0, 15);

	this->init(64);
	this->serializeMultiple<int64_t>({ 22, 23, 24, 25 }, 36, 9);
	this->serializeMultiple<int64_t>({ 26, 27, 28, 29, 30 }, 0, 9);
	this->serializeMultiple<int64_t>({ 31, 32, 33, 34 }, 0, 9);
	this->serializeMultiple<int64_t>({ 35, 36, 37 }, 28, 16);

}

TEST_F(serializeToInt64, SerializeMultiple)
{

	this->init(64);

	this->serializeMultiple<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8 }, 36, 5);
	this->serializeMultiple<int64_t>({ 9, 10, 11 }, 28, 8);

	this->init(64);
	this->serializeMultiple<int32_t>({ 12, 13, 14, 15, 16 }, 24, 6);
	this->serializeMultiple<int64_t>({ 17, 18, 19, 20 }, 36, 15);

	this->serializeMultiple<int32_t>({ 21 }, 0, 15);

	this->init(64);
	this->serializeMultiple<int64_t>({ 22, 23, 24, 25 }, 36, 9);
	this->serializeMultiple<int64_t>({ 26, 27, 28, 29, 30 }, 0, 9);
	this->serializeMultiple<int64_t>({ 31, 32, 33, 34 }, 0, 9);
	this->serializeMultiple<int64_t>({ 35, 36, 37 }, 28, 16);

}

TEST_F(serializeToInt32, Deserialize)
{

	this->init(64);

	const std::vector<int32_t> in1{ { 1, 2, 3, 4, 5, 6, 7, 8 } };
	const std::vector<int64_t> in2{ { 9, 10, 11 } };

	for (const auto in : in1)
		this->serialize(in, sizeof(in), 0);

	for (const auto in : in2)
		this->serialize(in, sizeof(in), 0);

	this->header = 0;
	for (int32_t id{}; id < in1.size(); ++id)
		this->deserialize(4, id + 1);

	for (int32_t id{}; id < in2.size(); ++id)
		this->deserialize<int64_t>(8, 8 + id + 1);

}

TEST_F(serializeToInt64, Deserialize)
{

	this->init(64);

	const std::vector<int32_t> in1{ { 1, 2, 3, 4, 5, 6, 7, 8 } };
	const std::vector<int64_t> in2{ { 9, 10, 11 } };

	for (const auto in : in1)
		this->serialize(in, sizeof(in), 0);

	for (const auto in : in2)
		this->serialize(in, sizeof(in), 0);

	this->header = 0;
	for (int32_t id{}; id < in1.size(); ++id)
		this->deserialize(4, id + 1);

	for (int32_t id{}; id < in2.size(); ++id)
		this->deserialize<int64_t>(8, 8 + id + 1);

}

TEST_F(serializeToInt32, DeserializeMultiple)
{

	this->init(64);

	const std::vector<int32_t> in1{ { 1, 2, 3, 4, 5, 6, 7, 8 } };
	const std::vector<int64_t> in2{ { 9, 10, 11 } };

	this->serializeMultiple(in1, in1.size() * 4 + 4, 0);
	this->serializeMultiple(in2, in2.size() * 8 + 4, 0);

	this->header = 0;
	this->deserializeMultiple(in1.size() * 4 + 4, in1);
	this->deserializeMultiple(in2.size() * 8 + 4, in2);

}

TEST_F(serializeToInt64, DeserializeMultiple)
{

	this->init(64);

	const std::vector<int32_t> in1{ { 1, 2, 3, 4, 5, 6, 7, 8 } };
	const std::vector<int64_t> in2{ { 9, 10, 11 } };

	this->serializeMultiple(in1, in1.size() * 4 + 4, 0);
	this->serializeMultiple(in2, in2.size() * 8 + 4, 0);

	this->header = 0;
	this->deserializeMultiple(in1.size() * 4 + 4, in1);
	this->deserializeMultiple(in2.size() * 8 + 4, in2);

}