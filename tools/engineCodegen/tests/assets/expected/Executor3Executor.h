#pragma once

#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)

namespace Game::ExecutionBlock
{
    class Executor3
    {
    public:
        struct ExternalContexts
        {
        };

        struct CExternalContexts
        {
        };

    public:
        void loadContext() noexcept;
        void initiate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;
        [[nodiscard]] CExternalContexts getExternalContext() noexcept;

    private:
        void initImpl(ExternalContexts& externalContexts) noexcept;
        void updateImpl(ExternalContexts& externalContexts, float dt) noexcept;

    public:
        Dod::MemPool memory;
        Context::SContext1::CData shared1Context;
        Context::SContext2::CData shared3Context;
    };
}

#pragma warning(pop)
