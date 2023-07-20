#include "Executor2Executor.h"

namespace Game::ExecutionBlock
{

    void Executor2::loadContext() noexcept
    {
        this->local1Context.load();
        this->local2Context.load();
        this->local3Context.load();

        this->target1Context.load();
        this->target2Context.load();
        this->target3Context.load();
    }

    void Executor2::initiate() noexcept
    {
        this->initImpl();
    }

    void Executor2::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Executor2::flushSharedLocalContexts() noexcept
    {
        this->target2Context.reset();
    }

}
