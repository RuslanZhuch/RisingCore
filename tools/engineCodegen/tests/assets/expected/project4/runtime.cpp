
#include <dod/SharedContext.h>
#include <engine/Timer.h>

int main()
{


    Timer timer;
    float deltaTime{};
    float targetDeltaTime{ 1.f / 60.f };
    while(true)
    {
        if (deltaTime >= targetDeltaTime)
        {






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
