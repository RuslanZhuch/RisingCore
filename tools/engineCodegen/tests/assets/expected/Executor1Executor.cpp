#include "Executor1Executor.h"

namespace Game::ExecutionBlock
{

    void Executor1::loadContext() noexcept
    {
    }

    void Executor1::initiate() noexcept
    {
        this->initImpl();
    }

    void Executor1::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Executor1::flushSharedLocalContexts() noexcept
    {
    }

}
