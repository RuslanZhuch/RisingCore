#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>
#include <executors/Executor3Executor.h>

#include <dod/SharedContext.h>
#include <chrono>

int main()
{
    Game::Context::SContext1::Data sharedInst1Context;
    Game::Context::SContext1::Data sharedInst2Context;
    Game::Context::SContext2::Data sharedInst3Context;
    Game::Context::SContext2::Data sharedInst4Context;
    sharedInst1Context.load();
    sharedInst2Context.load();
    sharedInst3Context.load();
    sharedInst4Context.load();

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.initiate();
    Game::ExecutionBlock::Executor3 executor3;
    executor3.loadContext();
    executor3.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        const auto computedP1_sharedInst1Context{ Game::Context::SContext1::convertToConst(sharedInst1Context) };
        const auto computedP1_sharedInst2Context{ Game::Context::SContext1::convertToConst(sharedInst2Context) };
        const auto computedP1_sharedInst3Context{ Game::Context::SContext2::convertToConst(sharedInst3Context) };
        const auto computedP1_sharedInst4Context{ Game::Context::SContext2::convertToConst(sharedInst4Context) };

        executor1.update(deltaTime);
        executor2.shared1Context = computedP1_sharedInst1Context;
        executor2.shared2Context = computedP1_sharedInst2Context;
        executor2.shared3Context = computedP1_sharedInst3Context;
        executor2.shared4Context = computedP1_sharedInst4Context;
        executor2.update(deltaTime);
        executor3.shared1_2Context = computedP1_sharedInst1Context;
        executor3.shared2_2Context = computedP1_sharedInst3Context;
        executor3.update(deltaTime);

        sharedInst1Context.reset();
        sharedInst2Context.reset();
        sharedInst4Context.reset();

        executor2.modifyTarget1(sharedInst1Context);
        executor2.modifyTarget2(sharedInst1Context);
        executor3.modifyShared3(sharedInst3Context);

        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();
        executor3.flushSharedLocalContexts();

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
