#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>

#include <dod/SharedContext.h>
#include <engine/Timer.h>

namespace
{
}

namespace
{
    Game::ExecutionBlock::Executor1 executor1;
    Game::ExecutionBlock::Executor2 executor2;
}

struct DependenciesMask
{

    uint64_t part1{};
};

int main()
{

    executor1.loadContext();
    executor1.initiate();
    executor2.loadContext();
    executor2.initiate();

    Timer timer;
    float deltaTime{};
    while(true)
    {
        if (deltaTime >= 1.f / 60.f)
        {


            executor1.update(deltaTime);
            executor2.update(deltaTime);



            executor1.flushSharedLocalContexts();
            executor2.flushSharedLocalContexts();

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
