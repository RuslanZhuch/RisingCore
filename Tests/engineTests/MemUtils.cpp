#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/MemUtils.h>


#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

struct DataHolder
{
	
	Dod::MemTypes::dataPoint_t dataBegin{ nullptr };
	Dod::MemTypes::dataPoint_t dataEnd{ nullptr };

};


TEST(MemUtils, Acquiring)
{

	{
		DataHolder holder;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 10 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
	}

	DataHolder holder;
	std::array<Dod::MemTypes::data_t, 32> data;
	holder.dataBegin = data.data();
	holder.dataEnd = data.data() + data.size();

	{
		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 10 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, data.data());
		EXPECT_EQ(acquiredEnd, data.data() + endIndex);
	}

	{
		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, data.data());
		EXPECT_EQ(acquiredEnd, data.data() + endIndex);
	}

	{
		constexpr Dod::MemTypes::capacity_t beginIndex{ 10 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 15 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, data.data() + beginIndex);
		EXPECT_EQ(acquiredEnd, data.data() + endIndex);
	}

	{
		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 40 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, acquiredEnd);
	}

	{
		constexpr Dod::MemTypes::capacity_t beginIndex{ 32 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 10 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, acquiredEnd);
	}

	{
		constexpr Dod::MemTypes::capacity_t beginIndex{ 15 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 10 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::aquire(holder, beginIndex, endIndex) };
		EXPECT_EQ(acquiredBegin, acquiredEnd);
	}

}

TEST(MemUtils, StackAcquiring)
{

	{
		int32_t header{ 0 };
		DataHolder holder;

		constexpr auto bytesToAcquire{ 10 };
		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(header, 0);
	}

	{
		int32_t header{ 0 };
		std::array<Dod::MemTypes::data_t, 32> data;
		DataHolder holder;

		constexpr auto bytesToAcquire{ 10 };
		holder.dataBegin = nullptr;
		holder.dataEnd = data.data() + bytesToAcquire;

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(header, 0);
	}

	{
		int32_t header{ 0 };
		std::array<Dod::MemTypes::data_t, 32> data;
		DataHolder holder;

		constexpr auto bytesToAcquire{ 10 };
		holder.dataBegin = data.data();
		holder.dataEnd = nullptr;

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(header, 0);
	}

	{
		int32_t header{ 0 };
		std::array<Dod::MemTypes::data_t, 32> data;
		DataHolder holder;

		constexpr auto bytesToAcquire{ 0 };
		holder.dataBegin = data.data();
		holder.dataEnd = data.data() + bytesToAcquire;

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(header, 0);
	}

	int32_t header{ 0 };
	std::array<Dod::MemTypes::data_t, 32> data;
	DataHolder holder;
	holder.dataBegin = data.data();
	holder.dataEnd = data.data() + data.size();

	auto totalAcquired{ 0 };

	{
		constexpr auto bytesToAcquire{ 10 };

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, holder.dataBegin);
		EXPECT_EQ(acquiredEnd, holder.dataBegin + bytesToAcquire);
		EXPECT_EQ(holder.dataBegin + header, acquiredEnd);

		totalAcquired += bytesToAcquire;
	}

	{
		constexpr auto bytesToAcquire{ 15 };

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, holder.dataBegin + totalAcquired);
		EXPECT_EQ(acquiredEnd, holder.dataBegin + bytesToAcquire + totalAcquired);
		EXPECT_EQ(holder.dataBegin + header, acquiredEnd);

		totalAcquired += bytesToAcquire;
	}

	{
		constexpr auto bytesToAcquire{ 10 };

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(holder.dataBegin + header, holder.dataBegin + totalAcquired);
	}

	{
		constexpr auto bytesToAcquire{ 7 };

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, holder.dataBegin + totalAcquired);
		EXPECT_EQ(acquiredEnd, holder.dataBegin + bytesToAcquire + totalAcquired);
		EXPECT_EQ(holder.dataBegin + header, acquiredEnd);

		totalAcquired += bytesToAcquire;
	}

	{
		constexpr auto bytesToAcquire{ 1 };

		const auto [acquiredBegin, acquiredEnd] { Dod::MemUtils::stackAquire(holder, bytesToAcquire, header) };
		EXPECT_EQ(acquiredBegin, nullptr);
		EXPECT_EQ(acquiredEnd, nullptr);
		EXPECT_EQ(holder.dataBegin + header, holder.dataBegin + totalAcquired);
	}

}
