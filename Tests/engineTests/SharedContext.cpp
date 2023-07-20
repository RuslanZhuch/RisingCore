#include "pch.h"

#include <dod/BufferUtils.h>
#include <dod/SharedContext.h>
#include <dod/MemPool.h>

#include <array>

struct Context1
{
	float data1{};
	float data2{};

	Dod::DBBuffer<int32_t> buffer1;

	void load()
	{

		this->memory.allocate(1024);
		int32_t header{};

		this->data1 = 42.f;
		this->data2 = 123.f;

		Dod::BufferUtils::initFromMemory(this->buffer1, Dod::MemUtils::stackAquire(this->memory, 10 * sizeof(int32_t), header));
		Dod::BufferUtils::populate(this->buffer1, 2, true);
		Dod::BufferUtils::populate(this->buffer1, 3, true);
		Dod::BufferUtils::populate(this->buffer1, 4, true);
		Dod::BufferUtils::populate(this->buffer1, 5, true);
		Dod::BufferUtils::populate(this->buffer1, 6, true);
		Dod::BufferUtils::populate(this->buffer1, 7, true);
		Dod::BufferUtils::populate(this->buffer1, 8, true);
		Dod::BufferUtils::populate(this->buffer1, 9, true);
		Dod::BufferUtils::populate(this->buffer1, 10, true);
	}

	void reset()
	{
		Dod::BufferUtils::flush(this->buffer1);
	}

	void merge(const Context1& other)
	{
		Dod::BufferUtils::append(this->buffer1, Dod::BufferUtils::createImFromBuffer(other.buffer1));
	}

	Dod::MemPool memory;

};

TEST(SharedContext, readData)
{

	Dod::SharedContext::Controller<Context1> controller;

	EXPECT_EQ(Dod::SharedContext::get(&controller).data1, 42.f);
	EXPECT_EQ(Dod::SharedContext::get(&controller).data2, 123.f);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(Dod::SharedContext::get(&controller).buffer1), 9);

	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 5);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 4), 6);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 5), 7);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 6), 8);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 7), 9);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 8), 10);

}

TEST(SharedContext, flushData)
{

	Dod::SharedContext::Controller<Context1> controller;
	Dod::SharedContext::flush(&controller);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(Dod::SharedContext::get(&controller).buffer1), 0);

}

TEST(SharedContext, mergeData)
{

	Dod::SharedContext::Controller<Context1> controller;
	Dod::SharedContext::flush(&controller);
	for (int32_t elId{}; elId < 1; ++elId)
		Dod::BufferUtils::populate(controller.context.buffer1, elId + 1, true);

	Context1 localContext1;
	localContext1.load();
	localContext1.reset();
	for (int32_t elId{}; elId < 3; ++elId)
		Dod::BufferUtils::populate(localContext1.buffer1, elId + 2, true);

	Context1 localContext2;
	localContext2.load();
	localContext2.reset();
	for (int32_t elId{}; elId < 4; ++elId)
		Dod::BufferUtils::populate(localContext2.buffer1, elId + 5, true);

	Context1 localContext3;
	localContext3.load();
	localContext3.reset();

	Dod::SharedContext::merge(&controller, localContext1);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(controller.context.buffer1), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 4);

	Dod::SharedContext::merge(&controller, localContext3);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(controller.context.buffer1), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 4);

	Dod::SharedContext::merge(&controller, localContext2);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(controller.context.buffer1), 8);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 4), 5);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 5), 6);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 6), 7);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 7), 8);

}
