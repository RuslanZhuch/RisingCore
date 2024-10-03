#include <gtest/gtest.h>

#include <engine/LocalMemoryBuffer.h>

#include <array>

namespace
{
	template <typename TSource>
	[[nodiscard]] bool getIsMemoryCorrect(const Engine::LocalMemory::Buffer& memory, const std::span<const TSource> srcData, size_t elId, size_t offset = 0) {
		return *(reinterpret_cast<TSource*>(memory.data + offset) + elId) == srcData[elId];
	};
}

TEST(LocalMemoryBuffer, creation)
{

	constexpr auto getIsAligned = [](Dod::MemTypes::data_t* data, int alignment) {
		return (reinterpret_cast<std::uintptr_t>(data) % alignment) == 0;
	};

	{
		constexpr auto size{ 64 };
		Engine::LocalMemory::Buffer memory(size);
		EXPECT_TRUE(getIsAligned(memory.data, 64));
		EXPECT_EQ(memory.numOfBytes, size);
	}
	{
		constexpr auto size{ 128 };
		Engine::LocalMemory::Buffer memory(size);
		EXPECT_TRUE(getIsAligned(memory.data, 64));
		EXPECT_EQ(memory.numOfBytes, size);
	}
	{
		constexpr auto size{ 1024 * 1024 };
		Engine::LocalMemory::Buffer memory(size);
		EXPECT_TRUE(getIsAligned(memory.data, 64));
		EXPECT_EQ(memory.numOfBytes, size);
	}
	{
		constexpr auto size{ 1024 * 1024 * 1024 };
		Engine::LocalMemory::Buffer memory(size);
		EXPECT_TRUE(getIsAligned(memory.data, 64));
		EXPECT_EQ(memory.numOfBytes, size);
	}

}

TEST(LocalMemoryBuffer, setData)
{



	{
		constexpr auto size{ 64 };
		Engine::LocalMemory::Buffer memory(size);

		using type_t = int;
		const auto data{ std::to_array<type_t>({1, 2, 3, 4, 5, 6, 7, 8 }) };
		size_t bytesOffset{};
		Engine::LocalMemory::set<type_t>(memory, data, bytesOffset);
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 0));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 1));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 2));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 3));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 4));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 5));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 6));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 7));

	}
	{
		constexpr auto size{ 64 };
		Engine::LocalMemory::Buffer memory(size);

		using type_t = int;
		const auto data{ std::to_array<type_t>({1, 2, 3, 4, 5, 6, 7, 8 }) };
		size_t bytesOffset{ 16 };
		Engine::LocalMemory::set<type_t>(memory, data, bytesOffset);

		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 0, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 1, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 2, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 3, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 4, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 5, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 6, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 7, bytesOffset));

	}
	{
		constexpr auto size{ 64 };
		Engine::LocalMemory::Buffer memory(size);

		using type_t = int64_t;
		const auto data{ std::to_array<type_t>({1, 2, 3, 4, 5, 6, 7, 8 }) };
		size_t bytesOffset{};
		Engine::LocalMemory::set<type_t>(memory, data, bytesOffset);

		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 0));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 1));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 2));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 3));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 4));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 5));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 6));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 7));

	}
	{
		constexpr auto size{ 64 };
		Engine::LocalMemory::Buffer memory(size);

		using type_t = int64_t;
		const auto data{ std::to_array<type_t>({1, 2, 3, 4, 5, 6, 7, 8 }) };
		size_t bytesOffset{16};
		Engine::LocalMemory::set<type_t>(memory, data, bytesOffset);

		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 0, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 1, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 2, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 3, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 4, bytesOffset));
		EXPECT_TRUE(getIsMemoryCorrect<type_t>(memory, data, 5, bytesOffset));

	}
	{
		constexpr auto size{ 64 };
		Engine::LocalMemory::Buffer memory(size);

		using type_t = int64_t;
		const auto data{ std::to_array<type_t>({1, 2, 3, 4, 5, 6, 7, 8 }) };
		size_t bytesOffset{ 128 };
		Engine::LocalMemory::set<type_t>(memory, data, bytesOffset);

	}

}
