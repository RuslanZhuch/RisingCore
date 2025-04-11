#include "Executor2Executor.h"

namespace Game::ExecutionBlock
{

    static Executor2::ExternalContexts externalContexts;

    void Executor2::loadContext() noexcept
    {
        this->local1Context.load();
        this->local2Context.load();
        this->local3Context.load();

    }

    void Executor2::initiate() noexcept
    {
        this->initImpl(externalContexts);
    }

    void Executor2::deactivate() noexcept
    {
        if (this->bIsRunning)
        {
            this->bIsRunning = false;
            this->onDeactivatedImpl(externalContexts);
        }
    }

    void Executor2::update(float dt) noexcept
    {
        if (!this->bIsRunning)
        {
            this->bIsRunning = true;
            this->onActivatedImpl(externalContexts);
        }
        this->updateImpl(externalContexts, dt);
    }

    void Executor2::flushSharedLocalContexts() noexcept
    {
        this->target2Context.reset();
    }

}
