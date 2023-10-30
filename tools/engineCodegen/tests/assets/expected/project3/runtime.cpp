#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>
#include <executors/Executor3Executor.h>
#include <executors/Executor4Executor.h>
#include <executors/Executor5Executor.h>
#include <executors/Executor6Executor.h>

#include <dod/SharedContext.h>
#include <dod/BufferUtils.h>
#include <chrono>

int main()
{
    Game::Context::SContext1::Data poolInst1Context;
    Game::Context::SContext1::Data poolInst2Context;
    Game::Context::SContext2::Data poolInst3Context;

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.initiate();
    Game::ExecutionBlock::Executor3 executor3;
    executor3.loadContext();
    executor3.initiate();
    Game::ExecutionBlock::Executor4 executor4;
    executor4.loadContext();
    executor4.initiate();
    Game::ExecutionBlock::Executor5 executor5;
    executor5.loadContext();
    executor5.initiate();
    Game::ExecutionBlock::Executor6 executor6;
    executor6.loadContext();
    executor6.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };



        executor1.update(deltaTime);
        executor2.update(deltaTime);

        poolInst1Context.merge(executor1.pool1Context);
        poolInst2Context.merge(executor1.pool2Context);
        poolInst2Context.merge(executor2.pool2Context);

        const auto computedP2_poolInst1Context{ Game::Context::SContext1::convertToConst(poolInst1Context) };
        const auto computedP2_poolInst2Context{ Game::Context::SContext1::convertToConst(poolInst2Context) };
        const auto computedP2_poolInst3Context{ Game::Context::SContext2::convertToConst(poolInst3Context) };

        executor3.pool1Context = computedP2_poolInst1Context;
        executor3.update(deltaTime);
        executor4.pool2Context = computedP2_poolInst2Context;
        executor4.pool3Context = computedP2_poolInst3Context;
        executor4.update(deltaTime);

        poolInst3Context.merge(executor4.pool3Context);

        const auto computedP3_poolInst2Context{ Game::Context::SContext1::convertToConst(poolInst2Context) };
        const auto computedP3_poolInst3Context{ Game::Context::SContext2::convertToConst(poolInst3Context) };

        executor5.pool2Context = computedP3_poolInst2Context;
        executor5.pool3Context = computedP3_poolInst3Context;
        executor5.update(deltaTime);
        executor6.pool3Context = computedP3_poolInst3Context;
        executor6.update(deltaTime);

        poolInst1Context.reset();
        poolInst2Context.reset();
        poolInst3Context.reset();


        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();
        executor3.flushSharedLocalContexts();
        executor4.flushSharedLocalContexts();
        executor5.flushSharedLocalContexts();
        executor6.flushSharedLocalContexts();

        for (int32_t cmdId{}; cmdId < Dod::DataUtils::getNumFilledElements(sApplicationContext.commands); ++cmdId)
        {
            if (Dod::DataUtils::get(sApplicationContext.commands, 0) == 1)
            {
                return 0;
            }
        }

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }
}
