#include <gtest/gtest.h>

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
		EXPECT_EQ(capacity.numOfBytes, 3072);
		EXPECT_EQ(capacity.numOfElements, 256);
	}
//	{
//		Dod::MemPool memory;
//		memory.allocate(2048);
//		Dod::MemTypes::capacity_t header{};
//
//		Dod::DTable<double, int, bool> dst;
//		Engine::ContextUtils::CapacityData capacityData(1077, 81);
//		Engine::ContextUtils::initData(dst, capacityData, memory, header);
//		EXPECT_EQ(Dod::DataUtils::getCapacity(dst), 81);
//	}

}

TEST(Context, InitTable)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileTables.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 2);

	{
		Dod::MemPool memory;
		memory.allocate(2048);
		Dod::MemTypes::capacity_t header{};

		Dod::DTable<float, int> dst;
		Engine::ContextUtils::CapacityData capacityData(384, 41);
		Engine::ContextUtils::initData(dst, capacityData, memory, header);
		EXPECT_EQ(Dod::DataUtils::getCapacity(dst), 41);
	}
	{
		Dod::MemPool memory;
		memory.allocate(2048);
		Dod::MemTypes::capacity_t header{};

		Dod::DTable<double, int, bool> dst;
		Engine::ContextUtils::CapacityData capacityData(704+384+128, 81);
		Engine::ContextUtils::initData(dst, capacityData, memory, header);
		EXPECT_EQ(Dod::DataUtils::getCapacity(dst), 81);
	}

}

struct BufferType1
{
	float x;
	int y;

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

template <typename ... DataTypes>
class ContextTableTest : public ::testing::Test {

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

		const auto destCapacity{ Dod::DataUtils::computeCapacityInBytes<std::tuple<DataTypes...>>(numOfDestElements) };
		Engine::ContextUtils::CapacityData capacityData(destCapacity, numOfDestElements);
		Engine::ContextUtils::initData(this->dst, capacityData, this->memory, header);
		ASSERT_EQ(Dod::DataUtils::getCapacity(this->dst), numOfDestElements);

		Engine::ContextUtils::loadDataContent(this->dst, data.GetArray(), dataId);

	}

	Dod::MemPool memory;
	rapidjson::Document doc;
	Dod::DTable<DataTypes...> dst;

};

using ContextMonoTableTestTrivial = ContextTableTest<int32_t>;
using ContextMonoTableTestComplex = ContextTableTest<BufferType1>;
using ContextTableTestComplex = ContextTableTest<BufferType1, int32_t, float>;

TEST_F(ContextMonoTableTestTrivial, LoadMonoTableContentDataIsTrivial)
{

	this->run(4, 4);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);

	EXPECT_EQ(Dod::DataUtils::get(this->dst, 0), 1);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 1), 2);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 2), 3);
	EXPECT_EQ(Dod::DataUtils::get(this->dst, 3), 4);

}

TEST_F(ContextMonoTableTestComplex, LoadMonoTableContent)
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

TEST_F(ContextTableTestComplex, LoadTableContent)
{

	this->run(4, 5);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);

	const auto col0{ Dod::DataUtils::get<0>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col0, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).y, 6);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).x, 9.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).y, 10);
	EXPECT_EQ(Dod::DataUtils::get(col0, 3).x, 13.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 3).y, 14);

	const auto col1{ Dod::DataUtils::get<1>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col1, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(col1, 1), 7);
	EXPECT_EQ(Dod::DataUtils::get(col1, 2), 11);
	EXPECT_EQ(Dod::DataUtils::get(col1, 3), 15);

	const auto col2{ Dod::DataUtils::get<2>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col2, 0), 4);
	EXPECT_EQ(Dod::DataUtils::get(col2, 1), 8);
	EXPECT_EQ(Dod::DataUtils::get(col2, 2), 12);
	EXPECT_EQ(Dod::DataUtils::get(col2, 3), 16);

}

TEST_F(ContextTableTestComplex, LoadTableContentDestIsLarger)
{

	this->run(6, 5);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 4);

	const auto col0{ Dod::DataUtils::get<0>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col0, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).y, 6);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).x, 9.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).y, 10);
	EXPECT_EQ(Dod::DataUtils::get(col0, 3).x, 13.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 3).y, 14);

	const auto col1{ Dod::DataUtils::get<1>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col1, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(col1, 1), 7);
	EXPECT_EQ(Dod::DataUtils::get(col1, 2), 11);
	EXPECT_EQ(Dod::DataUtils::get(col1, 3), 15);

	const auto col2{ Dod::DataUtils::get<2>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col2, 0), 4);
	EXPECT_EQ(Dod::DataUtils::get(col2, 1), 8);
	EXPECT_EQ(Dod::DataUtils::get(col2, 2), 12);
	EXPECT_EQ(Dod::DataUtils::get(col2, 3), 16);

}

TEST_F(ContextTableTestComplex, LoadTableContentDestIsSmaller)
{

	this->run(3, 5);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 3);

	const auto col0{ Dod::DataUtils::get<0>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col0, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 0).y, 2);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).y, 6);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).x, 9.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).y, 10);

	const auto col1{ Dod::DataUtils::get<1>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col1, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(col1, 1), 7);
	EXPECT_EQ(Dod::DataUtils::get(col1, 2), 11);

	const auto col2{ Dod::DataUtils::get<2>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col2, 0), 4);
	EXPECT_EQ(Dod::DataUtils::get(col2, 1), 8);
	EXPECT_EQ(Dod::DataUtils::get(col2, 2), 12);

}

TEST_F(ContextTableTestComplex, LoadTableContentDestIsEmpty)
{

	this->run(0, 5);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 0);

}

TEST_F(ContextTableTestComplex, LoadTableContentDataIsEmpty)
{

	this->run(4, 1);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 0);

}

TEST_F(ContextTableTestComplex, LoadTableContentDataNoField)
{

	this->run(4, 3);

	ASSERT_EQ(Dod::DataUtils::getNumFilledElements(this->dst), 0);

}

TEST_F(ContextTableTestComplex, LoadTableContentDataMissing)
{

	this->run(4, 6);

	const auto col0{ Dod::DataUtils::get<0>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col0, 0).x, 1.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 0).y, 0);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).x, 5.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 1).y, 6);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).x, 0.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 2).y, 10);
	EXPECT_EQ(Dod::DataUtils::get(col0, 3).x, 0.f);
	EXPECT_EQ(Dod::DataUtils::get(col0, 3).y, 0);

	const auto col1{ Dod::DataUtils::get<1>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col1, 0), 3);
	EXPECT_EQ(Dod::DataUtils::get(col1, 1), 7);
	EXPECT_EQ(Dod::DataUtils::get(col1, 2), 11);
	EXPECT_EQ(Dod::DataUtils::get(col1, 3), 15);

	const auto col2{ Dod::DataUtils::get<2>(this->dst) };

	EXPECT_EQ(Dod::DataUtils::get(col2, 0), 4);
	EXPECT_EQ(Dod::DataUtils::get(col2, 1), 8);
	EXPECT_EQ(Dod::DataUtils::get(col2, 2), 12);
	EXPECT_EQ(Dod::DataUtils::get(col2, 3), 16);

}
