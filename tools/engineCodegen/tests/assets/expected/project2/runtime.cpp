#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>

#include <dod/SharedContext.h>
#include <engine/Timer.h>

int main()
{

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.initiate();

    Timer timer;
    float deltaTime{};
    float targetDeltaTime{ 1.f / 60.f };
    while(true)
    {
        if (deltaTime >= targetDeltaTime)
        {


            executor1.update(deltaTime);
            executor2.update(deltaTime);



            executor1.flushSharedLocalContexts();
            executor2.flushSharedLocalContexts();

            Dod::DataUtils::forEach(Dod::ImTable(sApplicationContext.data), [&](const Types::Application::Data& data)
            {
                if (data.cmd == 1)
                {
                    exit(0);
                }
                if (data.frameDeltaStep != 0)
                {
                    targetDeltaTime = data.frameDeltaStep;
                }
            }
            );
            deltaTime = {};
        }

        timer.tick();
        deltaTime += timer.getDeltaTime();
    }
}
