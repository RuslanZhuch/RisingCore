#include "pch.h"

#include <engine/contextUtils.h>
#include <engine/String.h>

struct Type2f
{

	float x{};
	float y{};

	void setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
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

struct TypeIntWith2f
{

	int int1{};
	Type2f inner;

	void setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
	{
		if (name == "int1")
		{
			Engine::ContextUtils::assignToVariable(this->int1, value);
			return;
		}
		if (name == "inner")
		{
			Engine::ContextUtils::assignToVariable(this->inner, value);
			return;
		}
	}

};

struct TypeComplex1
{

	int int1{};
	float float1{};
	Engine::String string1;
	Type2f inner;

	void setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
	{
		if (name == "int1")
		{
			Engine::ContextUtils::assignToVariable(this->int1, value);
			return;
		}
		if (name == "float1")
		{
			Engine::ContextUtils::assignToVariable(this->float1, value);
			return;
		}
		if (name == "string1")
		{
			Engine::ContextUtils::assignToVariable(this->string1, value);
			return;
		}
		if (name == "inner")
		{
			Engine::ContextUtils::assignToVariable(this->inner, value);
			return;
		}
	}

};

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
