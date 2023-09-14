#include "pch.h"

#include "utils/CommonCheckers.h"

#include <dod/MemTypes.h>
#include <dod/MemUtils.h>

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

TEST(GetAlignOffset, MemUtils)
{

	{
		const auto point{ reinterpret_cast<int*>(16) };
		EXPECT_EQ(Dod::MemUtils::getAlignOffset(point, 4), 0);
		EXPECT_EQ(Dod::MemUtils::getAlignOffset(point, 8), 0);
		EXPECT_EQ(Dod::MemUtils::getAlignOffset(point, 16), 0);
		EXPECT_EQ(Dod::MemUtils::getAlignOffset(point, 32), 16);
		EXPECT_EQ(Dod::MemUtils::getAlignOffset(point, 24), 8);
	}

}

struct DataHolder
{
	
	Dod::MemTypes::dataPoint_t dataBegin{ nullptr };
	Dod::MemTypes::dataPoint_t dataEnd{ nullptr };

};

class AcquireMemory : public ::testing::Test {

protected:
	void run(Dod::MemTypes::capacity_t beginIndex, Dod::MemTypes::capacity_t numOfBytes, Dod::MemTypes::alignment_t alignment, Dod::MemTypes::alignment_t expectedOffset)
	{

		DataHolder holder;
		holder.dataBegin = data.data();
		holder.dataEnd = data.data() + data.size();

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::acquire(holder, beginIndex, numOfBytes, alignment) };
		beginIndex = std::max(decltype(beginIndex){}, beginIndex);
		numOfBytes = std::max(decltype(numOfBytes){}, numOfBytes);
		EXPECT_EQ(acquiredBegin, data.data() + beginIndex + expectedOffset);
		EXPECT_EQ(acquiredEnd, data.data() + beginIndex + numOfBytes + expectedOffset);

		checkMemoryAligned(acquiredBegin, alignment);

	}

	void runFailed(Dod::MemTypes::capacity_t beginIndex, Dod::MemTypes::capacity_t numOfBytes, Dod::MemTypes::alignment_t alignment)
	{

		DataHolder holder;
		holder.dataBegin = data.data();
		holder.dataEnd = data.data() + data.size();

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::acquire(holder, beginIndex, numOfBytes, alignment) };
		EXPECT_EQ(acquiredBegin, acquiredEnd);

	}
	void runFailedNoMem(Dod::MemTypes::capacity_t beginIndex, Dod::MemTypes::capacity_t numOfBytes, Dod::MemTypes::alignment_t alignment)
	{

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::acquire(holder, beginIndex, numOfBytes, alignment) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
	}

	alignas(8) std::array<Dod::MemTypes::data_t, 32> data{};
	DataHolder holder;

};

TEST_F(AcquireMemory, Acquiring)
{

	this->run(0, 10, 1, 0);
	this->run(0, 32, 1, 0);
	this->run(10, 5, 1, 0);
	this->run(-10, 5, 1, 0);
	this->runFailed(0, 40, 1);
	this->runFailed(32, 0, 1);
	this->runFailed(15, -10, 1);
	this->runFailedNoMem(0, 10, 1);

	this->run(0, 10, 2, 0);
	this->run(0, 32, 2, 0);
	this->run(10, 5, 2, 0);
	this->run(-10, 5, 2, 0);
	this->runFailed(0, 40, 2);
	this->runFailed(32, 0, 2);
	this->runFailed(15, -10, 2);
	this->runFailedNoMem(0, 10, 2);

	this->run(0, 10, 4, 0);
	this->run(0, 32, 4, 0);
	this->run(10, 5, 4, 2);
	this->run(0, 5, 4, 0);
	this->runFailed(0, 40, 4);
	this->runFailed(32, 0, 4);
	this->runFailed(15, -10, 4);
	this->runFailedNoMem(0, 10, 4);

	this->run(0, 10, 8, 0);
	this->run(0, 32, 8, 0);
	this->run(10, 5, 8, 6);
	this->run(-10, 5, 8, 0);
	this->runFailed(0, 40, 8);
	this->runFailed(32, 0, 8);
	this->runFailed(15, -10, 8);
	this->runFailedNoMem(0, 10, 8);

}


class AcquireMemoryStack : public ::testing::Test {

protected:
	void runFailed(DataHolder holder, Dod::MemTypes::capacity_t bytesToAcquire, Dod::MemTypes::alignment_t alignment)
	{

		Dod::MemTypes::capacity_t header{ 0 };

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, alignment, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(header, 0);

	}

	void run(Dod::MemTypes::capacity_t bytesToAcquire, Dod::MemTypes::alignment_t alignment, Dod::MemTypes::alignment_t expectedOffset)
	{

		DataHolder holder;
		holder.dataBegin = data.data();
		holder.dataEnd = data.data() + data.size();

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, alignment, header) };
		EXPECT_EQ(acquiredBegin, holder.dataBegin + totalAcquired + expectedOffset);
		EXPECT_EQ(acquiredEnd, holder.dataBegin + bytesToAcquire + totalAcquired + expectedOffset);
		EXPECT_EQ(holder.dataBegin + header, acquiredEnd);

		this->totalAcquired += (bytesToAcquire + expectedOffset);

	}

	void runFailedSpace(Dod::MemTypes::capacity_t bytesToAcquire, Dod::MemTypes::alignment_t alignment)
	{

		DataHolder holder;
		holder.dataBegin = data.data();
		holder.dataEnd = data.data() + data.size();

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, alignment, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(holder.dataBegin + header, holder.dataBegin + this->totalAcquired);

	}

	Dod::MemTypes::capacity_t totalAcquired{};
	Dod::MemTypes::capacity_t header{ 0 };
	alignas(8) std::array<Dod::MemTypes::data_t, 32> data{};

};

TEST_F(AcquireMemoryStack, Acquiring)
{

	this->runFailed({}, 10, 1);
	this->runFailed(DataHolder(nullptr, this->data.data() + 10), 10, 1);
	this->runFailed(DataHolder(this->data.data(), nullptr), 10, 1);
	this->runFailed(DataHolder(this->data.data(), this->data.data() + 0), 0, 1);

	this->run(10, 1, 0);
	this->run(15, 1, 0);
	this->runFailedSpace(10, 1);
	this->run(7, 1, 0);
	this->runFailedSpace(1, 1);

	this->header = 0;
	this->totalAcquired = 0;

	this->run(10, 2, 0);
	this->run(15, 2, 0);
	this->runFailedSpace(10, 2);
	this->run(6, 2, 1);
	this->runFailedSpace(1, 2);

	this->header = 0;
	this->totalAcquired = 0;

	this->run(29, 2, 0);
	this->runFailedSpace(3, 2);
	this->run(2, 2, 1);

	this->header = 0;
	this->totalAcquired = 0;

	this->run(12, 1, 0);
	this->run(12, 8, 4);
	this->runFailedSpace(1, 8);

}
