
#include <pch.h>

#include <engine/String.h>
#include <engine/StringUtils.h>

TEST(String, defaultCreation)
{

	Engine::String str{};
	ASSERT_EQ(str.capacity, 64);

	ASSERT_TRUE(str == "");
	
}

TEST(String, Assignment)
{

	Engine::String str{};
	ASSERT_EQ(str.capacity, 64);

	ASSERT_TRUE(str == "");

	Engine::StringUtils::assign(str, "String test");

	ASSERT_TRUE(str == "String test");

	Engine::StringUtils::assign(str, "New string data");
	ASSERT_TRUE(str == "New string data");

}

TEST(String, Compare)
{
	Engine::String str1{};
	Engine::StringUtils::assign(str1, "String 1");
	Engine::String str2{};
	Engine::StringUtils::assign(str2, "String 1");
	Engine::String str3{};
	Engine::StringUtils::assign(str3, "String 3");

	EXPECT_EQ(str1, str2);
	EXPECT_TRUE(str1 != str3);
	EXPECT_TRUE(str2 != str3);
}
