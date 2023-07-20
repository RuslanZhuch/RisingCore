#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>

#include <dod/SharedContext.h>
#include <dod/BufferUtils.h>
#include <chrono>

int main()
{

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        executor1.update(deltaTime);
        executor2.update(deltaTime);



        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();

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
