#include "pch.h"

#include <dod/Buffers.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
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
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 6) };

	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	EXPECT_EQ(data.Size(), 6);

}

TEST(Context, LoadDataFailed)
{

	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 7) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}
	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}
	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileNotExist.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}

}

TEST(Context, LoadVariable)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 6) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 6);

	{
		float dst{};
		Engine::ContextUtils::loadVariable(dst, data, 0);
		EXPECT_EQ(dst, 0.f);
	}

	{
		float dst{};
		Engine::ContextUtils::loadVariable(dst, data, 1);
		EXPECT_EQ(dst, 50.f);
	}

	{
		int32_t dst{};
		Engine::ContextUtils::loadVariable(dst, data, 2);
		EXPECT_EQ(dst, 25);
	}

	{
		uint32_t dst{};
		Engine::ContextUtils::loadVariable(dst, data, 3);
		EXPECT_EQ(dst, 134217728U);
	}

	{
		SampleStringType dst;
		Engine::ContextUtils::loadVariable(dst, data, 4);
		EXPECT_EQ(*dst, "Title text");
	}

	{
		TypeComplex1 dst;
		Engine::ContextUtils::loadVariable(dst, data, 5);
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
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 3);

	{
		using type_t = float;
		const auto capacity{ Engine::ContextUtils::getBufferCapacity<type_t>(data, 0) };
		EXPECT_EQ(capacity.numOfBytes, 41 * sizeof(type_t));
		EXPECT_EQ(capacity.numOfElements, 41);
	}
	{
		using type_t = int64_t;
		const auto capacity{ Engine::ContextUtils::getBufferCapacity<type_t>(data, 1) };
		EXPECT_EQ(capacity.numOfBytes, 81 * sizeof(type_t));
		EXPECT_EQ(capacity.numOfElements, 81);
	}
	{
		const auto capacity{ Engine::ContextUtils::getDataCapacity<int, double>(data, 2) };
		EXPECT_EQ(capacity.numOfBytes, 3100);
		EXPECT_EQ(capacity.numOfElements, 256);
	}

}

TEST(Context, InitBuffer)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileBuffers.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 3);

	{
		Dod::MemPool memory;
		memory.allocate(2048);
		Dod::MemTypes::capacity_t header{};

		Dod::DBBuffer<float> dst;
		Engine::ContextUtils::CapacityData capacityData(sizeof(float) * 41, 0);
		Engine::ContextUtils::initData(dst, capacityData, memory, header);
		EXPECT_EQ(Dod::DataUtils::getCapacity(dst), 40);
	}
	{
		Dod::MemPool memory;
		memory.allocate(2048);
		Dod::MemTypes::capacity_t header{};

		Dod::DBBuffer<int64_t> dst;
		Engine::ContextUtils::CapacityData capacityData(sizeof(int64_t) * 81, 0);
		Engine::ContextUtils::initData(dst, capacityData, memory, header);
		EXPECT_EQ(Dod::DataUtils::getCapacity(dst), 80);
	}
	{
		Dod::MemPool memory;
		memory.allocate(3116);
		Dod::MemTypes::capacity_t header{};

		Dod::DBTable<int, double> dst;
		Engine::ContextUtils::CapacityData capacityData(3100, 256);
		Engine::ContextUtils::initData(dst, capacityData, memory, header);
		EXPECT_EQ(Dod::DataUtils::getCapacity(dst), 256);
	}

}

struct BufferType1
{
	float x{};
	int y{};

	void setValueByName(std::string_view name, const rapidjson::Value& value)
	{
		if (name == "x")
		{
			Engine::ContextUtils::assignToVariable(this->x, value);
			return;
		}
		if (name == "y")
		{
			Engine::ContextUtils::assignToVariable(this->y, value);
			return;
		}
	}

};

template <typename DataType>
class ContextTest : public ::testing::Test {

protected:
	void SetUp() override 
	{

		this->doc = Engine::ContextUtils::loadFileDataRoot("assets/typeArray.json");
		ASSERT_TRUE(doc.IsObject());

	}

	void run(int32_t numOfDestElements, int32_t dataId)
	{

		const auto obj{ this->doc.GetObject() };

		const auto& data{ obj["data"] };
		ASSERT_TRUE(data.IsArray());

		this->memory.allocate(2048);
		Dod::MemTypes::capacity_t header{};

		const auto destCapacity{ numOfDestElements + 1 };
		Engine::ContextUtils::CapacityData capacityData(sizeof(DataType) * destCapacity, 0);
		Engine::ContextUtils::initData(this->dst, capacityData, this->memory, header);
		ASSERT_EQ(Dod::DataUtils::getCapacity(this->dst), numOfDestElements);

		Engine::ContextUtils::loadDataContent(this->dst, data.GetArray(), dataId);
	
	}

	Dod::MemPool memory;
	rapidjson::Document doc;
	Dod::DBBuffer<DataType> dst;

};

using ContextTestComplex = ContextTest<BufferType1>;
using ContextTestTrivial = ContextTest<int32_t>;

TEST_F(ContextTestComplex, LoadBufferContent)
{

	this->run(4, 0);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);
	
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).x, 3.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).y, 4);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).y, 6);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3).x, 7.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3).y, 8);

}

TEST_F(ContextTestComplex, LoadBufferContentDestIsLarger)
{

	this->run(6, 0);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);

	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).x, 3.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).y, 4);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).y, 6);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3).x, 7.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3).y, 8);

}

TEST_F(ContextTestComplex, LoadBufferContentDestIsSmaller)
{

	this->run(3, 0);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 3);

	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).x, 3.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).y, 4);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).y, 6);

}

TEST_F(ContextTestComplex, LoadBufferContentDestIsEmpty)
{

	this->run(0, 0);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 0);

}

TEST_F(ContextTestComplex, LoadBufferContentDataIsEmpty)
{

	this->run(4, 1);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 0);

}

TEST_F(ContextTestComplex, LoadBufferContentDataNoField)
{

	this->run(4, 3);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 0);

}

TEST_F(ContextTestComplex, LoadBufferContentMappingIsWrong)
{

	this->run(4, 2);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);

	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).x, 3.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1).y, 4);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2).y, 0);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3).x, 0.f);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3).y, 0);

}

TEST_F(ContextTestTrivial, LoadBufferContentDataIsTrivial)
{

	this->run(4, 4);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);

	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1), 2);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2), 3);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3), 4);

}
