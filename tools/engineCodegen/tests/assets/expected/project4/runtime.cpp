
#include <dod/SharedContext.h>
#include <dod/BufferUtils.h>
#include <chrono>

int main()
{


    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };





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