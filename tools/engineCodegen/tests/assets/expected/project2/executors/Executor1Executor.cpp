#include "Executor1Executor.h"

namespace Game::ExecutionBlock
{

    static Executor1::ExternalContexts externalContexts;

    void Executor1::loadContext() noexcept
    {
    }

    void Executor1::initiate() noexcept
    {
        this->initImpl(externalContexts);
    }

    void Executor1::update(float dt) noexcept
    {
        this->updateImpl(externalContexts, dt);
    }

    void Executor1::flushSharedLocalContexts() noexcept
    {
    }

}
