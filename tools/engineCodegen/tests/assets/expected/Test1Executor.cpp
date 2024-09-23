#include "Test1Executor.h"

namespace Game::ExecutionBlock
{

    static Assets::ExternalContexts externalContexts;

    void Test1::loadContext() noexcept
    {
        this->local1Context.load();
        this->local2Context.load();
        this->local3Context.load();
        this->local4Context.load();

        externalContexts.external1Context.load();
        externalContexts.external2Context.load();
        externalContexts.external3Context.load();

    }

    void Test1::initiate() noexcept
    {
        this->initImpl(externalContexts);
    }

    void Test1::update(float dt) noexcept
    {
        this->updateImpl(externalContexts, dt);
    }

    void Test1::flushSharedLocalContexts() noexcept
    {
        this->target1Context.reset();
        this->target3Context.reset();
    }

    Assets::CExternalContexts Game::ExecutionBlock::Assets::getExternalContext() noexcept
    {
        return CExternalContexts
        {
            .external1Context = Context::Type7::convertToConst(externalContexts.external1Context),
            .external2Context = Context::Type7::convertToConst(externalContexts.external2Context),
            .external3Context = Context::Type8::convertToConst(externalContexts.external3Context),
        };
    }

}
