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
    Dod::SharedContext::Controller<Game::Context::SContext1::Data> poolInst1Context;
    Dod::SharedContext::Controller<Game::Context::SContext1::Data> poolInst2Context;
    Dod::SharedContext::Controller<Game::Context::SContext2::Data> poolInst3Context;

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.initiate();
    Game::ExecutionBlock::Executor3 executor3;
    executor3.loadContext();
    executor3.pool1Context = &poolInst1Context;
    executor3.initiate();
    Game::ExecutionBlock::Executor4 executor4;
    executor4.loadContext();
    executor4.pool2Context = &poolInst2Context;
    executor4.pool3Context = &poolInst3Context;
    executor4.initiate();
    Game::ExecutionBlock::Executor5 executor5;
    executor5.loadContext();
    executor5.pool2Context = &poolInst2Context;
    executor5.pool3Context = &poolInst3Context;
    executor5.initiate();
    Game::ExecutionBlock::Executor6 executor6;
    executor6.loadContext();
    executor6.pool3Context = &poolInst3Context;
    executor6.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };


        executor1.update(deltaTime);
        executor2.update(deltaTime);

        Dod::SharedContext::merge(&poolInst1Context, executor1.pool1Context);
        Dod::SharedContext::merge(&poolInst2Context, executor1.pool2Context);
        Dod::SharedContext::merge(&poolInst2Context, executor2.pool2Context);

        executor3.update(deltaTime);
        executor4.update(deltaTime);

        Dod::SharedContext::merge(&poolInst3Context, executor4.pool3Context);

        executor5.update(deltaTime);
        executor6.update(deltaTime);

        Dod::SharedContext::flush(&poolInst1Context);
        Dod::SharedContext::flush(&poolInst2Context);
        Dod::SharedContext::flush(&poolInst3Context);


        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();
        executor3.flushSharedLocalContexts();
        executor4.flushSharedLocalContexts();
        executor5.flushSharedLocalContexts();
        executor6.flushSharedLocalContexts();

        for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplicationContext.context.commands); ++cmdId)
        {
            if (Dod::BufferUtils::get(sApplicationContext.context.commands, 0) == 1)
            {
                return 0;
            }
        }

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }
}
