
#include <Contexts/EContextAContext.h>
#include <Contexts/EContextBContext.h>
#include <Contexts/SContextAContext.h>
#include <Contexts/SContextBContext.h>
#include <Contexts/SContextCContext.h>
#include <Contexts/SContextDContext.h>
#include <Contexts/SContextEContext.h>
#include <Contexts/SContextFContext.h>
#include <Contexts/SContextGContext.h>

#include <executors/Executor1Executor.h>
#include <executors/Executor2Executor.h>
#include <executors/Executor3Executor.h>
#include <executors/Executor4Executor.h>
#include <executors/Executor5Executor.h>
#include <executors/Executor6Executor.h>
#include <executors/Executor7Executor.h>
#include <executors/Executor8Executor.h>
#include <executors/Executor9Executor.h>

#include <dod/SharedContext.h>
#include <engine/Timer.h>
#include <engine/Bitmask.h>

namespace
{
    Game::Context::SContextA::Data contextAInst1Context;
    Game::Context::SContextB::Data contextBInst1Context;
    Game::Context::SContextC::Data contextCInst1Context;
    Game::Context::SContextC::Data contextDInst1Context;
    Game::Context::SContextD::Data contextEInst1Context;
    Game::Context::SContextE::Data contextFInst1Context;
    Game::Context::SContextE::Data contextGInst1Context;
    Game::Context::SContextF::Data contextHInst1Context;
    Game::Context::SContextF::Data contextIInst1Context;
    Game::Context::SContextF::Data contextJInst1Context;
    Game::Context::SContextG::Data contextKInst1Context;

    Game::Context::EContextA::CData extContextAInst1Context;
    Game::Context::EContextA::CData extContextAInst2Context;
    Game::Context::EContextB::CData extContextBInst1Context;
}

namespace
{
    Game::ExecutionBlock::Executor1 executor1;
    Game::ExecutionBlock::Executor2 executor2;
    Game::ExecutionBlock::Executor3 executor3;
    Game::ExecutionBlock::Executor4 executor4;
    Game::ExecutionBlock::Executor5 executor5;
    Game::ExecutionBlock::Executor6 executor6;
    Game::ExecutionBlock::Executor7 executor7;
    Game::ExecutionBlock::Executor8 executor8;
    Game::ExecutionBlock::Executor9 executor9;
}

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
    Engine::Bitmask::Bitmask<1> enabledExecutors{ };
}

namespace
{
    void mergeContextAInst1()
    {
        if (Engine::Bitmask::get(enabledExecutors, 2))
            executor8.modifyContext1Output(contextAInst1Context);
    }

    void mergeContextBInst1()
    {
        contextBInst1Context.reset();
        executor7.modifyContext2Output(contextBInst1Context);
    }

    void mergeContextCInst1()
    {
        if (Engine::Bitmask::get(enabledExecutors, 0))
            executor1.modifyContext3Output(contextCInst1Context);
    }

    void mergeContextDInst1()
    {
        executor2.modifyContext4Output(contextDInst1Context);
        if (Engine::Bitmask::get(enabledExecutors, 1))
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
        if (Engine::Bitmask::get(enabledExecutors, 1))
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
        const auto externalContexts{ executor2.getExternalContext() };
    }

    void tryRunExecutor4(float dt)
    {
        executor4.context4InputContext = Game::Context::ContextDInst1::convertToConst(contextDInst1Context);
        executor4.update(dt);
        const auto externalContexts{ executor4.getExternalContext() };
        extContextAInst2Context = externalContexts.extOutput1Context;
        extContextBInst1Context = externalContexts.extOutput2Context;
    }

    void tryRunExecutor5(float dt)
    {
        executor5.context5InputContext = Game::Context::ContextEInst1::convertToConst(contextEInst1Context);
        executor5.update(dt);
        const auto externalContexts{ executor5.getExternalContext() };
    }

    void tryRunExecutor6(float dt)
    {
        executor6.context6InputContext = Game::Context::ContextFInst1::convertToConst(contextFInst1Context);
        executor6.extInput1Context = extContextAInst1Context;
        executor6.extInput2Context = extContextBInst1Context;
        executor6.update(dt);
        const auto externalContexts{ executor6.getExternalContext() };
    }

    void tryRunExecutor7(float dt)
    {
        executor7.context7InputContext = Game::Context::ContextGInst1::convertToConst(contextGInst1Context);
        executor7.update(dt);
        const auto externalContexts{ executor7.getExternalContext() };
    }

    void tryRunExecutor9(float dt)
    {
        executor9.update(dt);
        const auto externalContexts{ executor9.getExternalContext() };
    }

    void tryRunExecutor1(float dt)
    {
        if (!Engine::Bitmask::get(enabledExecutors, 0))
            return;
        executor1.context1InputContext = Game::Context::ContextAInst1::convertToConst(contextAInst1Context);
        executor1.update(dt);
        const auto externalContexts{ executor1.getExternalContext() };
        extContextAInst1Context = externalContexts.extOutput1Context;
    }

    void tryRunExecutor3(float dt)
    {
        if (!Engine::Bitmask::get(enabledExecutors, 1))
            return;
        executor3.context3InputContext = Game::Context::ContextCInst1::convertToConst(contextCInst1Context);
        executor3.update(dt);
        const auto externalContexts{ executor3.getExternalContext() };
    }

    void tryRunExecutor8(float dt)
    {
        if (!Engine::Bitmask::get(enabledExecutors, 2))
            return;
        executor8.context8InputContext = Game::Context::ContextHInst1::convertToConst(contextHInst1Context);
        executor8.context10InputContext = Game::Context::ContextJInst1::convertToConst(contextJInst1Context);
        executor8.extInput1Context = extContextAInst2Context;
        executor8.update(dt);
        const auto externalContexts{ executor8.getExternalContext() };
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
    contextAInst1Context.load();
    contextBInst1Context.load();
    contextCInst1Context.load();
    contextDInst1Context.load();
    contextEInst1Context.load();
    contextFInst1Context.load();
    contextGInst1Context.load();
    contextHInst1Context.load();
    contextIInst1Context.load();
    contextJInst1Context.load();
    contextKInst1Context.load();

    executor1.loadContext();
    executor1.initiate();
    executor2.loadContext();
    executor2.initiate();
    executor3.loadContext();
    executor3.initiate();
    executor4.loadContext();
    executor4.initiate();
    executor5.loadContext();
    executor5.initiate();
    executor6.loadContext();
    executor6.initiate();
    executor7.loadContext();
    executor7.initiate();
    executor8.loadContext();
    executor8.initiate();
    executor9.loadContext();
    executor9.initiate();

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
                const auto cmdData{ Dod::DataUtils::get(sApplicationContext.commands, cmdId) };
                const auto cmd{ cmdData & 0xF };
                if (cmd == 1)
                    return 0;
                if (cmd == 2 || cmd == 3)
                {
                    const auto executorCode{ cmdData >> 16 };
                    const auto bEnable{ cmd == 2 };
                    if (executorCode == 1)
                        Engine::Bitmask::set(enabledExecutors, 0, bEnable);
                    if (executorCode == 3)
                        Engine::Bitmask::set(enabledExecutors, 1, bEnable);
                    if (executorCode == 8)
                        Engine::Bitmask::set(enabledExecutors, 2, bEnable);
                }
            }
            deltaTime = {};
        }

        timer.tick();
        deltaTime += timer.getDeltaTime();
    }
}
