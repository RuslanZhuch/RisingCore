#include <gtest/gtest.h>

#include <engine/StaticVector.h>
#include <engine/VectorUtils.h>

#include <array>

TEST(StaticVector, defaultCreation)
{

	{
		Engine::StaticVector<int32_t, 32> vec{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(vec), 32);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 0);
	}

	{
		Engine::StaticVector<int32_t, 64> vec{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(vec), 64);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 0);
	}

	{
		Engine::StaticVector<double, 32> vec{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(vec), 32);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 0);
	}

}

struct LargeData
{
	int data[4];
};

void checkElements(auto&& vec, auto expected)
{
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(vec), expected.size());
	for (int32_t id{}; id < Engine::ContainerUtils::getNumOfElements(vec); ++id)
	{
		EXPECT_EQ(vec.data[id], expected[id]);
	}
}

TEST(StaticVector, populate)
{

	{
		Engine::StaticVector<int32_t, 4> vec{};

		Engine::ContainerUtils::pushBack(vec, 1);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 1);
		checkElements(vec, std::to_array<int32_t>({ 1 }));

		Engine::ContainerUtils::pushBack(vec, 2);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 2);
		checkElements(vec, std::to_array<int32_t>({ 1, 2 }));

		Engine::ContainerUtils::pushBack(vec, 3);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 3);
		checkElements(vec, std::to_array<int32_t>({ 1, 2, 3 }));

		Engine::ContainerUtils::pushBack(vec, 4);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 4);
		checkElements(vec, std::to_array<int32_t>({ 1, 2, 3, 4 }));

		Engine::ContainerUtils::pushBack(vec, 5);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 4);
		checkElements(vec, std::to_array<int32_t>({ 1, 2, 3, 4 }));
	}

	{
		Engine::StaticVector<LargeData, 4> vec{};

		Engine::ContainerUtils::pushBack(vec, LargeData{});
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 1);

		Engine::ContainerUtils::pushBack(vec, LargeData{});
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 2);

		Engine::ContainerUtils::pushBack(vec, LargeData{});
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 3);

		Engine::ContainerUtils::pushBack(vec, LargeData{});
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 4);

		Engine::ContainerUtils::pushBack(vec, LargeData{});
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 4);
	}

}

TEST(StaticVector, forEach)
{

	{
		Engine::StaticVector<int32_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3);

		int32_t expected{ 1 };
		Engine::ContainerUtils::forEach(vec, [&](int32_t element) {
			EXPECT_EQ(expected++, element);
		});
		EXPECT_EQ(expected, 4);
	}

	{
		Engine::StaticVector<double, 8> vec{};
		Engine::ContainerUtils::pushBack(vec, 1.);
		Engine::ContainerUtils::pushBack(vec, 2.);
		Engine::ContainerUtils::pushBack(vec, 3.);
		Engine::ContainerUtils::pushBack(vec, 4.);
		Engine::ContainerUtils::pushBack(vec, 5.);
		Engine::ContainerUtils::pushBack(vec, 6.);

		int32_t cnt{};
		double expected[]{ 1., 2., 3., 4., 5., 6, };
		Engine::ContainerUtils::forEach(vec, [&](double element) {
			EXPECT_EQ(expected[cnt], element);
			++cnt;
		});
		EXPECT_EQ(cnt, 6);
	}

}

TEST(StaticVector, clear)
{
	Engine::StaticVector<int32_t, 4> vec{};
	Engine::ContainerUtils::pushBack(vec, 1);
	Engine::ContainerUtils::pushBack(vec, 2);
	Engine::ContainerUtils::pushBack(vec, 3);

	Engine::ContainerUtils::clear(vec);

	ASSERT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 0);

	Engine::ContainerUtils::pushBack(vec, 4);
	Engine::ContainerUtils::pushBack(vec, 5);
	Engine::ContainerUtils::pushBack(vec, 6);
	Engine::ContainerUtils::pushBack(vec, 7);

	ASSERT_EQ(Engine::ContainerUtils::getCapacity(vec), 4);
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(vec), 4);

	checkElements(vec, std::to_array<int32_t>({ 4, 5, 6, 7 }));

}

TEST(StaticVector, assign)
{

	{
		Engine::StaticVector<int32_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3);

		Engine::StaticVector<int32_t, 4> vec2{};
		Engine::ContainerUtils::assign(vec2, vec);

		checkElements(vec2, std::to_array<int32_t>({ 1, 2, 3 }));
	}

	{
		Engine::StaticVector<int32_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3);

		Engine::StaticVector<int32_t, 3> vec2{};
		Engine::ContainerUtils::assign(vec2, vec);

		checkElements(vec2, std::to_array<int32_t>({ 1, 2, 3 }));
	}

	{
		Engine::StaticVector<int32_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3);

		Engine::StaticVector<int32_t, 2> vec2{};
		Engine::ContainerUtils::assign(vec2, vec);

		checkElements(vec2, std::to_array<int32_t>({ 1, 2 }));
	}

	{
		using type_t = std::variant<int32_t, float>;
		Engine::StaticVector<type_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3.f);

		checkElements(vec, std::to_array<type_t>({ 1, 2, 3.f }));
	}

	{
        struct BigData1
        {
            int data1;
            float data2;
            uint32_t data3;

            [[nodiscard]] auto operator<=>(const BigData1&) const = default;
        };

		struct BigData2
		{
			uint64_t data1;
			int32_t data2;

			[[nodiscard]] auto operator<=>(const BigData2&) const = default;
		};

		using type_t = std::variant<BigData1, BigData2>;
		Engine::StaticVector<type_t, 4> vec{};

		constexpr auto param1{ BigData1{1, 2.f, 3u} };
		constexpr auto param2{ BigData2{4ul, 5} };
		constexpr auto param3{ BigData1{6, 7.f, 8u} };

		Engine::ContainerUtils::pushBack(vec, param1);
		Engine::ContainerUtils::pushBack(vec, param2);
		Engine::ContainerUtils::pushBack(vec, param3);

		checkElements(vec, std::to_array<type_t>({ param1, param2, param3 }));

	}

}


TEST(StaticVector, getData)
{

	{
		Engine::StaticVector<int32_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3);

		const auto data{ Engine::ContainerUtils::getData(vec) };
		const auto expected{ std::to_array<int32_t>({1, 2, 3}) };
		EXPECT_EQ(data.size(), expected.size());
		EXPECT_TRUE(std::equal(data.begin(), data.end(), expected.begin(), expected.end()));
	}

	{
		Engine::StaticVector<int32_t, 4> vec{};
		Engine::ContainerUtils::pushBack(vec, 1);
		Engine::ContainerUtils::pushBack(vec, 2);
		Engine::ContainerUtils::pushBack(vec, 3);
		Engine::ContainerUtils::pushBack(vec, 4);

		const auto data{ Engine::ContainerUtils::getData(vec) };
		const auto expected{ std::to_array<int32_t>({1, 2, 3, 4}) };
		EXPECT_EQ(data.size(), expected.size());
		EXPECT_TRUE(std::equal(data.begin(), data.end(), expected.begin(), expected.end()));
	}

	{
		Engine::StaticVector<int32_t, 4> vec{};

		const auto data{ Engine::ContainerUtils::getData(vec) };
		EXPECT_TRUE(data.empty());
	}

//	{
//		Engine::StaticVector<int32_t, 4> vec{};
//		Engine::ContainerUtils::pushBack(vec, 1);
//		Engine::ContainerUtils::pushBack(vec, 2);
//		Engine::ContainerUtils::pushBack(vec, 3);
//
//		Engine::StaticVector<int32_t, 3> vec2{};
//		Engine::ContainerUtils::assign(vec2, vec);
//
//		checkElements(vec2, std::to_array<int32_t>({ 1, 2, 3 }));
//	}
//
//	{
//		Engine::StaticVector<int32_t, 4> vec{};
//		Engine::ContainerUtils::pushBack(vec, 1);
//		Engine::ContainerUtils::pushBack(vec, 2);
//		Engine::ContainerUtils::pushBack(vec, 3);
//
//		Engine::StaticVector<int32_t, 2> vec2{};
//		Engine::ContainerUtils::assign(vec2, vec);
//
//		checkElements(vec2, std::to_array<int32_t>({ 1, 2 }));
//	}

}
