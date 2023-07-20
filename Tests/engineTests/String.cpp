
#include <pch.h>

#include <engine/String.h>
#include <engine/StringUtils.h>

TEST(String, defaultCreation)
{

	Engine::String str;
	ASSERT_EQ(str.capacity, 64);

	ASSERT_TRUE(str == "");
	
}

TEST(String, Assignment)
{

	Engine::String str;
	ASSERT_EQ(str.capacity, 64);

	ASSERT_TRUE(str == "");

	Engine::StringUtils::assign(str, "String test");

	ASSERT_TRUE(str == "String test");

	Engine::StringUtils::assign(str, "New string data");
	ASSERT_TRUE(str == "New string data");

}
