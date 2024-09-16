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
    int16_t contextAInst1ExecutorsLeft{ 1 };
    int16_t contextBInst1ExecutorsLeft{ 1 };
    int16_t contextCInst1ExecutorsLeft{ 1 };
    int16_t contextDInst1ExecutorsLeft{ 2 };
    int16_t contextEInst1ExecutorsLeft{ 1 };
    int16_t contextFInst1ExecutorsLeft{ 2 };
    int16_t contextGInst1ExecutorsLeft{ 1 };
    int16_t contextHInst1ExecutorsLeft{ 2 };
    int16_t contextIInst1ExecutorsLeft{ 2 };
    int16_t contextKInst1ExecutorsLeft{ 1 };
};

namespace
{
    OptionalExecutorsMask enabledExecutors{};
}

namespace
{
    void mergeContextAInst1()
    {
        if (enabledExecutors.getIsEnabled(2))
            executor8.modifyContext1Output(contextAInst1Context);
    }

    void mergeContextBInst1()
    {
        contextBInst1Context.reset();
        executor7.modifyContext2Output(contextBInst1Context);
    }

    void mergeContextCInst1()
    {
        if (enabledExecutors.getIsEnabled(0))
            executor1.modifyContext3Output(contextCInst1Context);
    }

    void mergeContextDInst1()
    {
        executor2.modifyContext4Output(contextDInst1Context);
        if (enabledExecutors.getIsEnabled(1))
            executor3.modifyContext4Output(contextDInst1Context);
    }

    void mergeContextEInst1()
    {
        executor4.modifyContext5Output(contextEInst1Context);
    }

    void mergeContextFInst1()
    {
        executor2.modifyContext6Output(contextFInst1Context);
        executor4.modifyContext6Output(contextFInst1Context);
    }

    void mergeContextGInst1()
    {
        executor6.modifyContext7Output(contextGInst1Context);
    }

    void mergeContextHInst1()
    {
        contextHInst1Context.reset();
        executor5.modifyContext8Output(contextHInst1Context);
        executor7.modifyContext8Output(contextHInst1Context);
    }

    void mergeContextIInst1()
    {
        executor6.modifyContext9Output(contextIInst1Context);
        if (enabledExecutors.getIsEnabled(1))
            executor3.modifyContext9Output(contextIInst1Context);
    }

    void mergeContextKInst1()
    {
        executor2.modifyContext11Output(contextKInst1Context);
    }

}

namespace
{
    void tryRunExecutor2(float dt)
    {
        executor2.context2InputContext = Game::Context::ContextBInst1::convertToConst(contextBInst1Context);
        executor2.update(dt);
    }

    void tryRunExecutor4(float dt)
    {
        executor4.context4InputContext = Game::Context::ContextDInst1::convertToConst(contextDInst1Context);
        executor4.update(dt);
    }

    void tryRunExecutor5(float dt)
    {
        executor5.context5InputContext = Game::Context::ContextEInst1::convertToConst(contextEInst1Context);
        executor5.update(dt);
    }

    void tryRunExecutor6(float dt)
    {
        executor6.context6InputContext = Game::Context::ContextFInst1::convertToConst(contextFInst1Context);
        executor6.update(dt);
    }

    void tryRunExecutor7(float dt)
    {
        executor7.context7InputContext = Game::Context::ContextGInst1::convertToConst(contextGInst1Context);
        executor7.update(dt);
    }

    void tryRunExecutor9(float dt)
    {
        executor9.update(dt);
    }

    void tryRunExecutor1(float dt)
    {
        if (!enabledExecutors.getIsEnabled(0))
            return;
        executor1.context1InputContext = Game::Context::ContextAInst1::convertToConst(contextAInst1Context);
        executor1.update(dt);
    }

    void tryRunExecutor3(float dt)
    {
        if (!enabledExecutors.getIsEnabled(1))
            return;
        executor3.context3InputContext = Game::Context::ContextCInst1::convertToConst(contextCInst1Context);
        executor3.update(dt);
    }

    void tryRunExecutor8(float dt)
    {
        if (!enabledExecutors.getIsEnabled(2))
            return;
        executor8.context8InputContext = Game::Context::ContextHInst1::convertToConst(contextHInst1Context);
        executor8.context10InputContext = Game::Context::ContextJInst1::convertToConst(contextJInst1Context);
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
            --flowContext.contextCInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 0);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x2 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 1))
        {
            tryRunExecutor2(dt);
            --flowContext.contextDInst1ExecutorsLeft;
            --flowContext.contextFInst1ExecutorsLeft;
            --flowContext.contextKInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 1);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x4 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 2))
        {
            tryRunExecutor3(dt);
            --flowContext.contextDInst1ExecutorsLeft;
            --flowContext.contextIInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 2);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x8 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 3))
        {
            tryRunExecutor4(dt);
            --flowContext.contextEInst1ExecutorsLeft;
            --flowContext.contextFInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 3);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x10 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 4))
        {
            tryRunExecutor5(dt);
            --flowContext.contextHInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 4);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x20 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 5))
        {
            tryRunExecutor6(dt);
            --flowContext.contextGInst1ExecutorsLeft;
            --flowContext.contextIInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 5);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x40 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 6))
        {
            tryRunExecutor7(dt);
            --flowContext.contextBInst1ExecutorsLeft;
            --flowContext.contextHInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 6);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x280 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 7))
        {
            tryRunExecutor8(dt);
            --flowContext.contextAInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.completedExecutors, 7);
            return true;
        }

        if (Engine::Bitmask::includes(flowContext.contextsReadyMask, { 0x0 }) && !Engine::Bitmask::get(flowContext.completedExecutors, 8))
        {
            tryRunExecutor9(dt);
            Engine::Bitmask::set(flowContext.completedExecutors, 8);
            return true;
        }

        return false;
    }

    bool tryMergeNextContext(FlowContext& flowContext)
    {
        if (flowContext.contextAInst1ExecutorsLeft == 0)
        {
            mergeContextAInst1();
            --flowContext.contextAInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 0);
            return true;
        }

        if (flowContext.contextBInst1ExecutorsLeft == 0)
        {
            mergeContextBInst1();
            --flowContext.contextBInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 1);
            return true;
        }

        if (flowContext.contextCInst1ExecutorsLeft == 0)
        {
            mergeContextCInst1();
            --flowContext.contextCInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 2);
            return true;
        }

        if (flowContext.contextDInst1ExecutorsLeft == 0)
        {
            mergeContextDInst1();
            --flowContext.contextDInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 3);
            return true;
        }

        if (flowContext.contextEInst1ExecutorsLeft == 0)
        {
            mergeContextEInst1();
            --flowContext.contextEInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 4);
            return true;
        }

        if (flowContext.contextFInst1ExecutorsLeft == 0)
        {
            mergeContextFInst1();
            --flowContext.contextFInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 5);
            return true;
        }

        if (flowContext.contextGInst1ExecutorsLeft == 0)
        {
            mergeContextGInst1();
            --flowContext.contextGInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 6);
            return true;
        }

        if (flowContext.contextHInst1ExecutorsLeft == 0)
        {
            mergeContextHInst1();
            --flowContext.contextHInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 7);
            return true;
        }

        if (flowContext.contextIInst1ExecutorsLeft == 0)
        {
            mergeContextIInst1();
            --flowContext.contextIInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 8);
            return true;
        }

        if (flowContext.contextKInst1ExecutorsLeft == 0)
        {
            mergeContextKInst1();
            --flowContext.contextKInst1ExecutorsLeft;
            Engine::Bitmask::set(flowContext.contextsReadyMask, 10);
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
