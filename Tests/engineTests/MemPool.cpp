#include "pch.h"
#include <dod/MemPool.h>
#include <dod/MemPool.cpp>

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

TEST(MemPool, generation)
{

	const auto bytesToAllocate{ std::to_array({
		10, 100, 1000, 1'000'000
	}) };

	for (const auto bytes : bytesToAllocate)
	{
		Dod::MemPool pool;
		pool.allocate(bytes);
		const auto begin{ pool.dataBegin };
		const auto end{ pool.dataEnd };
		EXPECT_NE(begin, nullptr);
		EXPECT_NE(end, nullptr);
		EXPECT_EQ(end - begin, bytes);
	}

	{
		Dod::MemPool pool;
		const auto begin{ pool.dataBegin };
		const auto end{ pool.dataEnd };
		EXPECT_EQ(begin, nullptr);
		EXPECT_EQ(end, nullptr);
		EXPECT_EQ(end - begin, 0);
	}

}
