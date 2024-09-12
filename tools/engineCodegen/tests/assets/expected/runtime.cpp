#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>
#include <executors/Executor3Executor.h>

#include <dod/SharedContext.h>
#include <engine/Timer.h>
#include <engine/Bitmask.h>

namespace
{
    Game::Context::SContext1::Data sharedInst1Context;
    Game::Context::SContext1::Data sharedInst2Context;
    Game::Context::SContext2::Data sharedInst3Context;
    Game::Context::SContext2::Data sharedInst4Context;
}

namespace
{
    Game::ExecutionBlock::Executor1 executor1;
    Game::ExecutionBlock::Executor2 executor2;
    Game::ExecutionBlock::Executor3 executor3;
}

struct OptionalExecutorsMask
{
    auto getIsEnabled(int32_t executorId) 
    {
        const auto batchId{ executorId / 64 };
        const auto localIndex{ executorId % 64 };
        return this->mask[batchId] & (1 << localIndex) != 0;
    }
    uint64_t mask[1];
};

struct FlowContext
{
    Engine::Bitmask::Bitmask<1> contextsReadyMask{ 0x3 };
    Engine::Bitmask::Bitmask<1> completedExecutors{ };
    int16_t context1Inst1ExecutorsLeft{ 1 };
    int16_t context2Inst1ExecutorsLeft{ 1 };
    int16_t context3Inst1ExecutorsLeft{ 1 };
    int16_t context4Inst1ExecutorsLeft{ 2 };
    int16_t context5Inst1ExecutorsLeft{ 1 };
    int16_t context6Inst1ExecutorsLeft{ 2 };
    int16_t context7Inst1ExecutorsLeft{ 1 };
    int16_t context8Inst1ExecutorsLeft{ 2 };
    int16_t context9Inst1ExecutorsLeft{ 2 };
};

namespace
{
    OptionalExecutorsMask enabledExecutors{};
}

namespace
{
    void mergeContext1Inst1()
    {
        if (enabledExecutors.getIsEnabled(2))
            executor8.modifyContext1Output(context1Inst1Context);
    }

    void mergeContext2Inst1()
    {
        context2Inst1Context.reset();
        executor7.modifyContext2Output(context2Inst1Context);
    }

    void mergeContext3Inst1()
    {
        if (enabledExecutors.getIsEnabled(0))
            executor1.modifyContext3Output(context3Inst1Context);
    }

    void mergeContext4Inst1()
    {
        executor2.modifyContext4Output(context4Inst1Context);
        if (enabledExecutors.getIsEnabled(1))
            executor3.modifyContext4Output(context4Inst1Context);
    }

    void mergeContext5Inst1()
    {
        executor4.modifyContext5Output(context5Inst1Context);
    }

    void mergeContext6Inst1()
    {
        executor2.modifyContext6Output(context6Inst1Context);
        executor4.modifyContext6Output(context6Inst1Context);
    }

    void mergeContext7Inst1()
    {
        executor6.modifyContext7Output(context7Inst1Context);
    }

    void mergeContext8Inst1()
    {
        context8Inst1Context.reset();
        executor5.modifyContext8Output(context8Inst1Context);
        executor7.modifyContext8Output(context8Inst1Context);
    }

    void mergeContext9Inst1()
    {
        executor6.modifyContext9Output(context9Inst1Context);
        if (enabledExecutors.getIsEnabled(1))
            executor3.modifyContext9Output(context9Inst1Context);
    }

}

namespace
{
    void tryRunExecutor2(float dt)
    {
        executor2.update(dt);
    }

    void tryRunExecutor4(float dt)
    {
        executor4.update(dt);
    }

    void tryRunExecutor5(float dt)
    {
        executor5.update(dt);
    }

    void tryRunExecutor6(float dt)
    {
        executor6.update(dt);
    }

    void tryRunExecutor7(float dt)
    {
        executor7.update(dt);
    }

    void tryRunExecutor1(float dt)
    {
        if (!enabledExecutors.getIsEnabled(0))
            return;
        executor1.update(dt);
    }

    void tryRunExecutor3(float dt)
    {
        if (!enabledExecutors.getIsEnabled(1))
            return;
        executor3.update(dt);
    }

    void tryRunExecutor8(float dt)
    {
        if (!enabledExecutors.getIsEnabled(2))
            return;
        executor8.update(dt);
    }

}

namespace
{
    bool tryRunNextExecutor(FlowContext& flowContext, float dt)
    {
        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x1 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 0))
        {
            tryRunExecutor1(dt);
            --flowContext.context3Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 0);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x2 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 1))
        {
            tryRunExecutor2(dt);
            --flowContext.context4Inst1ExecutorsLeft;
            --flowContext.context6Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 1);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x4 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 2))
        {
            tryRunExecutor3(dt);
            --flowContext.context4Inst1ExecutorsLeft;
            --flowContext.context9Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 2);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x8 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 3))
        {
            tryRunExecutor4(dt);
            --flowContext.context5Inst1ExecutorsLeft;
            --flowContext.context6Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 3);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x10 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 4))
        {
            tryRunExecutor5(dt);
            --flowContext.context8Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 4);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x20 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 5))
        {
            tryRunExecutor6(dt);
            --flowContext.context7Inst1ExecutorsLeft;
            --flowContext.context9Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 5);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x40 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 6))
        {
            tryRunExecutor7(dt);
            --flowContext.context2Inst1ExecutorsLeft;
            --flowContext.context8Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 6);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x80 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 7))
        {
            tryRunExecutor8(dt);
            --flowContext.context1Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 7);
            return true;
        }

        return false;
    }

    bool tryMergeNextContext(FlowContext& flowContext)
    {
        if (flowContext.context1Inst1ExecutorsLeft == 0)
        {
            mergeContext1Inst1();
            --flowContext.context1Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 0);
            return true;
        }

        if (flowContext.context2Inst1ExecutorsLeft == 0)
        {
            mergeContext2Inst1();
            --flowContext.context2Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 1);
            return true;
        }

        if (flowContext.context3Inst1ExecutorsLeft == 0)
        {
            mergeContext3Inst1();
            --flowContext.context3Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 2);
            return true;
        }

        if (flowContext.context4Inst1ExecutorsLeft == 0)
        {
            mergeContext4Inst1();
            --flowContext.context4Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 3);
            return true;
        }

        if (flowContext.context5Inst1ExecutorsLeft == 0)
        {
            mergeContext5Inst1();
            --flowContext.context5Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 4);
            return true;
        }

        if (flowContext.context6Inst1ExecutorsLeft == 0)
        {
            mergeContext6Inst1();
            --flowContext.context6Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 5);
            return true;
        }

        if (flowContext.context7Inst1ExecutorsLeft == 0)
        {
            mergeContext7Inst1();
            --flowContext.context7Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 6);
            return true;
        }

        if (flowContext.context8Inst1ExecutorsLeft == 0)
        {
            mergeContext8Inst1();
            --flowContext.context8Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 7);
            return true;
        }

        if (flowContext.context9Inst1ExecutorsLeft == 0)
        {
            mergeContext9Inst1();
            --flowContext.context9Inst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 8);
            return true;
        }

        return false;
    }
}

int main()
{
    sharedInst1Context.load();
    sharedInst2Context.load();
    sharedInst3Context.load();
    sharedInst4Context.load();

    executor1.loadContext();
    executor1.initiate();
    executor2.loadContext();
    executor2.initiate();
    executor3.loadContext();
    executor3.initiate();

    Timer timer;
    float deltaTime{};
    while(true)
    {
        if (deltaTime >= 1.f / 60.f)
        {

            FlowContext flowContext{};

            while(true)
            {
                bool bChangeOccured{ false };
                bChangeOccured = tryRunNextExecutor(flowContext, deltaTime);
                bChangeOccured |= tryMergeNextContext(flowContext);

                if (!bChangeOccured)
                    break;
            }

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
