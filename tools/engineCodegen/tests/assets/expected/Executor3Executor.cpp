#include "Executor3Executor.h"

namespace Game::ExecutionBlock
{

    static Assets::ExternalContexts externalContexts;

    void Executor3::loadContext() noexcept
    {
    }

    void Executor3::initiate() noexcept
    {
        this->initImpl(externalContexts);
    }

    void Executor3::update(float dt) noexcept
    {
        this->updateImpl(externalContexts, dt);
    }

    void Executor3::flushSharedLocalContexts() noexcept
    {
    }

}
