#include "Test1Executor.h"

namespace Game::ExecutionBlock
{

    void Test1::loadContext() noexcept
    {
        this->local1Context.load();
        this->local2Context.load();
        this->local3Context.load();
        this->local4Context.load();

    }

    void Test1::initiate() noexcept
    {
        this->initImpl();
    }

    void Test1::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Test1::flushSharedLocalContexts() noexcept
    {
        this->target1Context.reset();
        this->target3Context.reset();
    }

}
