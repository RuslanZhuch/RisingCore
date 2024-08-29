#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>
#include <executors/Executor3Executor.h>
#include <executors/Executor4Executor.h>
#include <executors/Executor5Executor.h>
#include <executors/Executor6Executor.h>

#include <dod/SharedContext.h>
#include <engine/Timer.h>

namespace
{
    Game::Context::SContext1::Data poolInst1Context;
    Game::Context::SContext1::Data poolInst2Context;
    Game::Context::SContext2::Data poolInst3Context;
}

namespace
{
    Game::ExecutionBlock::Executor1 executor1;
    Game::ExecutionBlock::Executor2 executor2;
    Game::ExecutionBlock::Executor3 executor3;
    Game::ExecutionBlock::Executor4 executor4;
    Game::ExecutionBlock::Executor5 executor5;
    Game::ExecutionBlock::Executor6 executor6;
}

struct DependenciesMask
{

    uint64_t part1{};
};

int main()
{
    poolInst1Context.load();
    poolInst2Context.load();
    poolInst3Context.load();

    executor1.loadContext();
    executor1.initiate();
    executor2.loadContext();
    executor2.initiate();
    executor3.loadContext();
    executor3.initiate();
    executor4.loadContext();
    executor4.initiate();
    executor5.loadContext();
    executor5.initiate();
    executor6.loadContext();
    executor6.initiate();

    Timer timer;
    float deltaTime{};
    while(true)
    {
        if (deltaTime >= 1.f / 60.f)
        {



            executor1.update(deltaTime);
            executor2.update(deltaTime);

            executor1.modifyPool1(poolInst1Context);
            executor1.modifyPool2(poolInst2Context);
            executor2.modifyPool2(poolInst2Context);

            const auto computedP2_poolInst1Context{ Game::Context::SContext1::convertToConst(poolInst1Context) };
            const auto computedP2_poolInst2Context{ Game::Context::SContext1::convertToConst(poolInst2Context) };
            const auto computedP2_poolInst3Context{ Game::Context::SContext2::convertToConst(poolInst3Context) };

            executor3.pool1Context = computedP2_poolInst1Context;
            executor3.update(deltaTime);
            executor4.pool2Context = computedP2_poolInst2Context;
            executor4.pool3Context = computedP2_poolInst3Context;
            executor4.update(deltaTime);

            executor4.modifyPool3(poolInst3Context);

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
            deltaTime = {};
        }

        timer.tick();
        deltaTime += timer.getDeltaTime();
    }
}
