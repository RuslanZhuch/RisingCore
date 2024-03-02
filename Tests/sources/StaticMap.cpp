#include <gtest/gtest.h>

#include <engine/StaticMap.h>
#include <engine/MapUtils.h>

#include <array>

TEST(StaticMap, defaultCreation)
{

	{
		Engine::StaticMap<int32_t, double, 32> map{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 32);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 0);
	}

	{
		Engine::StaticMap<int32_t, double, 64> map{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 64);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 0);
	}

	{
		Engine::StaticMap<int32_t, float, 32> map{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 32);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 0);
	}

	{
		Engine::StaticMap<int64_t, float, 32> map{};
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 32);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 0);
	}

}

//struct LargeData
//{
//	int data[4];
//};
//
void checkElements(auto&& map, auto expectedKeys, auto expectedValues)
{
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), expectedKeys.size());
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), expectedValues.size());
	for (int32_t id{}; id < Engine::ContainerUtils::getNumOfElements(map); ++id)
	{
		EXPECT_EQ(map.data.keys[id], expectedKeys[id]);
	}
	for (int32_t id{}; id < Engine::ContainerUtils::getNumOfElements(map); ++id)
	{
		EXPECT_EQ(map.data.values[id], expectedValues[id]);
	}
}

TEST(StaticMap, populate)
{

	{
		Engine::StaticMap<int32_t, double, 4> map{};

		Engine::ContainerUtils::insert(map, 1, 1.);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(map), 1);
		checkElements(map, std::to_array<int32_t>({ 1 }), std::to_array<double>({ 1. }));

		Engine::ContainerUtils::insert(map, 2, 2.);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(map), 2);
		checkElements(map, std::to_array<int32_t>({ 1, 2 }), std::to_array<double>({ 1., 2. }));

		Engine::ContainerUtils::insert(map, 3, 3.);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(map), 3);
		checkElements(map, std::to_array<int32_t>({ 1, 2, 3 }), std::to_array<double>({ 1., 2., 3. }));

		Engine::ContainerUtils::insert(map, 4, 4.);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(map), 4);
		checkElements(map, std::to_array<int32_t>({ 1, 2, 3, 4 }), std::to_array<double>({ 1., 2., 3., 4. }));

		Engine::ContainerUtils::insert(map, 5, 5.);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(map), 4);
		checkElements(map, std::to_array<int32_t>({ 1, 2, 3, 4 }), std::to_array<double>({ 1., 2., 3., 4. }));

		Engine::ContainerUtils::insert(map, 3, 6.);
		EXPECT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		EXPECT_EQ(Engine::ContainerUtils::getNumOfElements(map), 4);
		checkElements(map, std::to_array<int32_t>({ 1, 2, 3, 4}), std::to_array<double>({ 1., 2., 6., 4 }));
	}

}

TEST(StaticMap, forEach)
{

	{
		Engine::StaticMap<int32_t, double, 4> map{};
		Engine::ContainerUtils::insert(map, 1, 1.);
		Engine::ContainerUtils::insert(map, 2, 2.);
		Engine::ContainerUtils::insert(map, 3, 3.);

		int32_t cnt{};
		double expected[]{ 1., 2., 3. };
		Engine::ContainerUtils::forEach(map, [&](double element) {
			EXPECT_EQ(expected[cnt], element);
			++cnt;
		});
		EXPECT_EQ(cnt, 3);
	}

	{
		Engine::StaticMap<int32_t, float, 6> map{};
		Engine::ContainerUtils::insert(map, 1, 1.);
		Engine::ContainerUtils::insert(map, 2, 2.);
		Engine::ContainerUtils::insert(map, 3, 3.);
		Engine::ContainerUtils::insert(map, 4, 4.);
		Engine::ContainerUtils::insert(map, 5, 5.);
		Engine::ContainerUtils::insert(map, 6, 6.);

		int32_t cnt{};
		float expected[]{ 1., 2., 3., 4., 5., 6, };
		Engine::ContainerUtils::forEach(map, [&](float element) {
			EXPECT_EQ(expected[cnt], element);
			++cnt;
		});
		EXPECT_EQ(cnt, 6);
	}

}

TEST(StaticMap, clear)
{
	Engine::StaticMap<int32_t, float, 4> map{};
	Engine::ContainerUtils::insert(map, 1, 1.f);
	Engine::ContainerUtils::insert(map, 2, 2.f);
	Engine::ContainerUtils::insert(map, 3, 3.f);

	Engine::ContainerUtils::clear(map);

	ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 0);

	Engine::ContainerUtils::insert(map, 4, 4.f);
	Engine::ContainerUtils::insert(map, 5, 5.f);
	Engine::ContainerUtils::insert(map, 6, 6.f);
	Engine::ContainerUtils::insert(map, 7, 7.f);

	ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
	ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 4);

	checkElements(map, std::to_array<int32_t>({ 4, 5, 6, 7 }), std::to_array<float>({ 4.f, 5.f, 6.f, 7.f }));

}

TEST(StaticMap, assign)
{

	{
		Engine::StaticMap<int32_t, float, 4> map{};
		Engine::ContainerUtils::insert(map, 1, 1.f);
		Engine::ContainerUtils::insert(map, 2, 2.f);
		Engine::ContainerUtils::insert(map, 3, 3.f);

		Engine::StaticMap<int32_t, float, 4> map2{};
		Engine::ContainerUtils::assign(map2, map);

		checkElements(map2, std::to_array<int32_t>({ 1, 2, 3 }), std::to_array<float>({ 1.f, 2.f, 3.f }));
	}

	{
		Engine::StaticMap<int32_t, float, 4> map{};
		Engine::ContainerUtils::insert(map, 1, 1.f);
		Engine::ContainerUtils::insert(map, 2, 2.f);
		Engine::ContainerUtils::insert(map, 3, 3.f);

		Engine::StaticMap<int32_t, float, 3> map2{};
		Engine::ContainerUtils::assign(map2, map);

		checkElements(map2, std::to_array<int32_t>({ 1, 2, 3 }), std::to_array<float>({ 1.f, 2.f, 3.f }));
	}

	{
		Engine::StaticMap<int32_t, float, 4> map{};
		Engine::ContainerUtils::insert(map, 1, 1.f);
		Engine::ContainerUtils::insert(map, 2, 2.f);
		Engine::ContainerUtils::insert(map, 3, 3.f);

		Engine::StaticMap<int32_t, float, 2> map2{};
		Engine::ContainerUtils::assign(map2, map);

		checkElements(map2, std::to_array<int32_t>({ 1, 2 }), std::to_array<float>({ 1.f, 2.f }));
	}

}

TEST(StaticMap, getOrInsert)
{

	{
		Engine::StaticMap<int32_t, float, 4> map{};

		EXPECT_EQ(Engine::ContainerUtils::getOrInsert(map, 1), 0);
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 1);
		checkElements(map, std::to_array<int32_t>({ 1 }), std::to_array<float>({ 0.f }));

		Engine::ContainerUtils::getOrInsert(map, 1) = 1.f;
		EXPECT_EQ(Engine::ContainerUtils::getOrInsert(map, 1), 1.f);
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 1);
		checkElements(map, std::to_array<int32_t>({ 1 }), std::to_array<float>({ 1.f }));

		Engine::ContainerUtils::insert(map, 1, 3.f);
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 1);
		checkElements(map, std::to_array<int32_t>({ 1 }), std::to_array<float>({ 3.f }));

		Engine::ContainerUtils::insert(map, 2, 4.f);
		EXPECT_EQ(Engine::ContainerUtils::getOrInsert(map, 1), 3.f);
		EXPECT_EQ(Engine::ContainerUtils::getOrInsert(map, 2), 4.f);
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);
		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 2);
		checkElements(map, std::to_array<int32_t>({ 1, 2 }), std::to_array<float>({ 3.f, 4.f }));
	}
}

TEST(StaticMap, get)
{

	{
		Engine::StaticMap<int32_t, float, 4> map{};
		Engine::ContainerUtils::insert(map, 1, 1.f);
		Engine::ContainerUtils::insert(map, 2, 2.f);
		Engine::ContainerUtils::insert(map, 3, 3.f);

		EXPECT_EQ(Engine::ContainerUtils::get(map, 1), 1.f);
		EXPECT_EQ(Engine::ContainerUtils::get(map, 2), 2.f);
		EXPECT_EQ(Engine::ContainerUtils::get(map, 3), 3.f);
		checkElements(map, std::to_array<int32_t>({ 1, 2, 3 }), std::to_array<float>({ 1.f, 2.f, 3.f }));

		Engine::ContainerUtils::get(map, 2) = 4.f;

		ASSERT_EQ(Engine::ContainerUtils::getNumOfElements(map), 3);
		ASSERT_EQ(Engine::ContainerUtils::getCapacity(map), 4);

		EXPECT_EQ(Engine::ContainerUtils::get(map, 1), 1.f);
		EXPECT_EQ(Engine::ContainerUtils::get(map, 2), 4.f);
		EXPECT_EQ(Engine::ContainerUtils::get(map, 3), 3.f);
		checkElements(map, std::to_array<int32_t>({ 1, 2, 3 }), std::to_array<float>({ 1.f, 4.f, 3.f }));

	}
}