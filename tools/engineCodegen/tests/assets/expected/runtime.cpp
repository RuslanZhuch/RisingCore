#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>
#include <executors/Executor3Executor.h>

#include <dod/SharedContext.h>
#include <dod/BufferUtils.h>
#include <chrono>

int main()
{
    Dod::SharedContext::Controller<Game::Context::SContext1::Data> sharedInst1Context;
    Dod::SharedContext::Controller<Game::Context::SContext1::Data> sharedInst2Context;
    Dod::SharedContext::Controller<Game::Context::SContext2::Data> sharedInst3Context;
    Dod::SharedContext::Controller<Game::Context::SContext2::Data> sharedInst4Context;

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.shared1Context = &sharedInst1Context;
    executor2.shared2Context = &sharedInst2Context;
    executor2.shared3Context = &sharedInst3Context;
    executor2.shared4Context = &sharedInst4Context;
    executor2.initiate();
    Game::ExecutionBlock::Executor3 executor3;
    executor3.loadContext();
    executor3.shared1_2Context = &sharedInst1Context;
    executor3.shared2_2Context = &sharedInst3Context;
    executor3.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        executor1.update(deltaTime);
        executor2.update(deltaTime);
        executor3.update(deltaTime);

        Dod::SharedContext::flush(&sharedInst1Context);
        Dod::SharedContext::flush(&sharedInst2Context);
        Dod::SharedContext::flush(&sharedInst4Context);

        Dod::SharedContext::merge(&sharedInst1Context, executor2.target1Context);
        Dod::SharedContext::merge(&sharedInst1Context, executor2.target2Context);
        Dod::SharedContext::merge(&sharedInst3Context, executor3.shared3Context);

        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();
        executor3.flushSharedLocalContexts();

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
