#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/Buffers.h>
#include <dod/BufferUtils.h>
#include <ranges>

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

TEST(Buffers, DBBuffer) 
{

	Dod::DBBuffer<int32_t> buffer;

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}

TEST(Buffers, ImBuffer)
{

	Dod::ImBuffer<int32_t> buffer;

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}

TEST(Buffers, MutBuffer)
{

	Dod::MutBuffer<int32_t> buffer;

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(buffer), 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}
