#include "pch.h"

#include <Helpers.h>

TEST(TypeHelpers, ConstexprFor)
{

	{
		int32_t counter = 0;

		RisingCore::Helpers::constexprLoop<42>([&]<size_t currentId>() {
			counter += 1;
		});

		EXPECT_EQ(counter, 42);
	}
	{
		int32_t counter = 10;

		RisingCore::Helpers::constexprLoop<100>([&]<size_t currentId>() {
			counter += 1;
		});

		EXPECT_EQ(counter, 110);
	}

}

TEST(TypeHelpers, FindLargestAlignment)
{

	{
		using types_t = std::tuple<int, double>;
		EXPECT_EQ(RisingCore::Helpers::findLargestAlignment<types_t>(), 8);
	}
	{
		using types_t = std::tuple<double, int>;
		EXPECT_EQ(RisingCore::Helpers::findLargestAlignment<types_t>(), 8);
	}
	{
		using types_t = std::tuple<int, int>;
		EXPECT_EQ(RisingCore::Helpers::findLargestAlignment<types_t>(), 4);
	}
	struct alignas(32) S32 {	};
	{
		using types_t = std::tuple<int, S32>;
		EXPECT_EQ(RisingCore::Helpers::findLargestAlignment<types_t>(), 32);
	}
	{
		using types_t = std::tuple<S32>;
		EXPECT_EQ(RisingCore::Helpers::findLargestAlignment<types_t>(), 32);
	}

}