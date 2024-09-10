#include <gtest/gtest.h>

#include <engine/Bitmask.h>

constexpr auto bitsPerBlock{ 64 };

TEST(Bitmask, bitmask1Block)
{

	const auto numOfBlocks{ 1 };
	const auto totalBits{ bitsPerBlock * numOfBlocks };
	Engine::Bitmask::Bitmask<numOfBlocks> bitmask{};


	for (int32_t bitId{}; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 0);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	for (int32_t bitId{1}; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 32);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 32));
	for (int32_t bitId{ 1 }; bitId < 32; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 33 }; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 63);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 32));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 63));
	for (int32_t bitId{ 1 }; bitId < 32; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 33 }; bitId < 63; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

}

TEST(Bitmask, bitmask2Blocks)
{

	const auto numOfBlocks{ 2 };
	const auto totalBits{ bitsPerBlock * numOfBlocks };
	Engine::Bitmask::Bitmask<numOfBlocks> bitmask{};

	for (int32_t bitId{}; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 0);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	for (int32_t bitId{ 1 }; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 32);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 32));
	for (int32_t bitId{ 1 }; bitId < 32; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 33 }; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 63);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 32));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 63));
	for (int32_t bitId{ 1 }; bitId < 32; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 33 }; bitId < 63; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 64 }; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 70);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 32));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 63));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 70));
	for (int32_t bitId{ 1 }; bitId < 32; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 33 }; bitId < 63; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 64 }; bitId < 70; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 71 }; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

	Engine::Bitmask::set(bitmask, 127);
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 32));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 63));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 70));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 127));
	for (int32_t bitId{ 1 }; bitId < 32; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 33 }; bitId < 63; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 64 }; bitId < 70; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 71 }; bitId < 127; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

}

TEST(Bitmask, bitmaskInitialization)
{

	const auto numOfBlocks{ 2 };
	const auto totalBits{ bitsPerBlock * numOfBlocks };
	Engine::Bitmask::Bitmask<numOfBlocks> bitmask{ 0x0000'0000'0000'0003, 0x0000'0000'0001'0002 };

	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 0));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 1));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 65));
	EXPECT_TRUE(Engine::Bitmask::get(bitmask, 80));


	for (int32_t bitId{ 2 }; bitId < 65; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 66 }; bitId < 80; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));
	for (int32_t bitId{ 81 }; bitId < totalBits; ++bitId)
		EXPECT_FALSE(Engine::Bitmask::get(bitmask, bitId));

}

TEST(Bitmask, bitmaskIncludes)
{

	const auto numOfBlocks{ 2 };
	const auto totalBits{ bitsPerBlock * numOfBlocks };
	Engine::Bitmask::Bitmask<numOfBlocks> bitmask{ 0x0000'0000'0000'0003, 0x0000'0000'0001'0002 };

	EXPECT_TRUE(Engine::Bitmask::includes(bitmask, { 0x0000'0000'0000'0003, 0x0000'0000'0001'0002 }));
	EXPECT_TRUE(Engine::Bitmask::includes(bitmask, { 0x0000'0000'0000'0003, 0x0000'0000'0000'0002 }));
	EXPECT_TRUE(Engine::Bitmask::includes(bitmask, { 0x0000'0000'0000'0003, 0x0000'0000'0001'0000 }));
	EXPECT_TRUE(Engine::Bitmask::includes(bitmask, { 0x0000'0000'0000'0000, 0x0000'0000'0001'0000 }));
	EXPECT_TRUE(Engine::Bitmask::includes(bitmask, { 0x0000'0000'0000'0000, 0x0000'0000'0000'0000 }));
	EXPECT_FALSE(Engine::Bitmask::includes(bitmask, { 0x0000'0000'0001'0003, 0x0000'0000'0001'0002 }));

}
