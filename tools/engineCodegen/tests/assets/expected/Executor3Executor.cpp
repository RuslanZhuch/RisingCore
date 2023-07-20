#include "Executor3Executor.h"

namespace Game::ExecutionBlock
{

    void Executor3::loadContext() noexcept
    {
    }

    void Executor3::initiate() noexcept
    {
        this->initImpl();
    }

    void Executor3::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Executor3::flushSharedLocalContexts() noexcept
    {
    }

}
