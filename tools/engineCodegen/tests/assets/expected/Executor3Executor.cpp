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

    void Executor3::deactivate() noexcept
    {
        if (this->bIsRunning)
        {
            this->bIsRunning = false;
            this->onDeactivatedImpl(externalContexts);
        }
    }

    void Executor3::update(float dt) noexcept
    {
        if (!this->bIsRunning)
        {
            this->bIsRunning = true;
            this->onActivatedImpl(externalContexts);
        }
        this->updateImpl(externalContexts, dt);
    }

    void Executor3::flushSharedLocalContexts() noexcept
    {
    }

}
