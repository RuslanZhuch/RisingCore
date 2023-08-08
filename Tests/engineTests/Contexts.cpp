#include "pch.h"

#include <dod/Buffers.h>
#include <dod/BufferUtils.h>
#include <engine/contextUtils.cpp>

#include "utils/TypeUtils.h"

#include <array>

struct SampleStringType
{

	[[nodiscard]] std::string_view operator*() const noexcept
	{
		return std::string_view(internalData.data());
	}

	static constexpr int32_t capacity{ 64 };

	std::array<char, capacity> internalData{};

};

TEST(Context, LoadJsonFile)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };

	ASSERT_TRUE(doc.IsObject());
	const auto obj{ doc.GetObject() };

	EXPECT_NE(obj.FindMember("whoIAm"), obj.end());
	EXPECT_NE(obj.FindMember("version"), obj.end());
	EXPECT_NE(obj.FindMember("contextType"), obj.end());
	EXPECT_NE(obj.FindMember("data"), obj.end());

}

TEST(Context, LoadJsonFileFailed)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileBroken.json") };

	ASSERT_FALSE(doc.IsObject());

}

TEST(Context, LoadJsonFileNoFile)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileDontExist.json") };

	ASSERT_FALSE(doc.IsObject());

}

TEST(Context, LoadData)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };

	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	EXPECT_EQ(data.Size(), 5);

}

TEST(Context, LoadDataFailed)
{

	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 6) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}
	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}
	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileNotExist.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}

}

TEST(Context, LoadVariable)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 5);

	{
		float dst{};
		Engine::ContextUtils::loadVariable(dst, data, 0);
		EXPECT_EQ(dst, 50.f);
	}

	{
		int32_t dst{};
		Engine::ContextUtils::loadVariable(dst, data, 1);
		EXPECT_EQ(dst, 25);
	}

	{
		uint32_t dst{};
		Engine::ContextUtils::loadVariable(dst, data, 2);
		EXPECT_EQ(dst, 134217728U);
	}

	{
		SampleStringType dst;
		Engine::ContextUtils::loadVariable(dst, data, 3);
		EXPECT_EQ(*dst, "Title text");
	}

	{
		TypeComplex1 dst;
		Engine::ContextUtils::loadVariable(dst, data, 4);
		EXPECT_EQ(dst.int1, 1);
		EXPECT_EQ(dst.float1, 2.f);
		EXPECT_STREQ(dst.string1.internalData.data(), "Some string 3");
		EXPECT_EQ(dst.inner.x, 4.f);
		EXPECT_EQ(dst.inner.y, 5.f);
	}

}

TEST(Context, LoadBufferCapacity)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileBuffers.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 2);

	{
		using type_t = float;
		const auto capacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<type_t>(data, 0) };
		EXPECT_EQ(capacityBytes, 41 * sizeof(type_t));
	}
	{
		using type_t = int64_t;
		const auto capacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<type_t>(data, 1) };
		EXPECT_EQ(capacityBytes, 81 * sizeof(type_t));
	}

}

TEST(Context, LoadBuffer)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileBuffers.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 2);

	{
		Dod::MemPool memory;
		memory.allocate(2048);
		int32_t header{};

		Dod::DBBuffer<float> dst;
		Engine::ContextUtils::loadBuffer(dst, sizeof(float) * 41, memory, header);
		EXPECT_EQ(Dod::BufferUtils::getCapacity(dst), 40);
	}
	{
		Dod::MemPool memory;
		memory.allocate(2048);
		int32_t header{};

		Dod::DBBuffer<int64_t> dst;
		Engine::ContextUtils::loadBuffer(dst, sizeof(int64_t) * 81, memory, header);
		EXPECT_EQ(Dod::BufferUtils::getCapacity(dst), 80);
	}

}
