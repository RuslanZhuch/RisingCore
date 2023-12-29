#include <gtest/gtest.h>

#include "utils/TypeUtils.h"

#include <engine/contextUtils.h>

TEST(Types, DeserializeType2f)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/typeSchemas.json") };

	ASSERT_TRUE(doc.IsObject());
	const auto obj{ doc.GetObject() };

	const auto& data{ obj["data"] };
	ASSERT_TRUE(data.IsArray());

	const auto& schema{ data.GetArray()[0] };
	ASSERT_TRUE(schema.IsObject());

	Type2f variable;
	Engine::ContextUtils::assignToVariable(variable, schema);
	EXPECT_EQ(variable.x, 1.f);
	EXPECT_EQ(variable.y, 2.f);

}

TEST(Types, DeserializeTypeNoSetValueByName)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/typeSchemas.json") };

	ASSERT_TRUE(doc.IsObject());
	const auto obj{ doc.GetObject() };

	const auto& data{ obj["data"] };
	ASSERT_TRUE(data.IsArray());

	const auto& schema{ data.GetArray()[0] };
	ASSERT_TRUE(schema.IsObject());

	TypeNoSetValueByName variable;
	Engine::ContextUtils::assignToVariable(variable, schema);
	EXPECT_EQ(variable.x, 0.f);
	EXPECT_EQ(variable.y, 0.f);
	EXPECT_EQ(variable.z, 0.f);

}

TEST(Types, DeserializeTypeIntWith2f)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/typeSchemas.json") };

	ASSERT_TRUE(doc.IsObject());
	const auto obj{ doc.GetObject() };

	const auto& data{ obj["data"] };
	ASSERT_TRUE(data.IsArray());

	const auto& schema{ data.GetArray()[1] };
	ASSERT_TRUE(schema.IsObject());

	TypeIntWith2f variable;
	Engine::ContextUtils::assignToVariable(variable, schema);
	EXPECT_EQ(variable.int1, 3);
	EXPECT_EQ(variable.inner.x, 4.f);
	EXPECT_EQ(variable.inner.y, 5.f);

}

TEST(Types, DeserializeTypeTypeComplex1)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/typeSchemas.json") };

	ASSERT_TRUE(doc.IsObject());
	const auto obj{ doc.GetObject() };

	const auto& data{ obj["data"] };
	ASSERT_TRUE(data.IsArray());

	const auto& schema{ data.GetArray()[2] };
	ASSERT_TRUE(schema.IsObject());

	TypeComplex1 variable;
	Engine::ContextUtils::assignToVariable(variable, schema);
	EXPECT_EQ(variable.int1, 6);
	EXPECT_EQ(variable.float1, 7);
	EXPECT_STREQ(variable.string1.internalData.data(), "Some string 8");
	EXPECT_EQ(variable.inner.x, 9.f);
	EXPECT_EQ(variable.inner.y, 10.f);

}
