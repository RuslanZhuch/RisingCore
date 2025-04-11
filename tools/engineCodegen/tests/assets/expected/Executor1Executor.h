#pragma once


#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)

namespace Game::ExecutionBlock
{
    class Executor1
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
        void deactivate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;
        [[nodiscard]] CExternalContexts getExternalContext() noexcept;

    private:
        void initImpl(ExternalContexts& externalContexts) noexcept;
        void updateImpl(ExternalContexts& externalContexts, float dt) noexcept;

        void onActivatedImpl(ExternalContexts& externalContexts) noexcept;
        void onDeactivatedImpl(ExternalContexts& externalContexts) noexcept;

    public:
        bool bIsRunning {};
        Dod::MemPool memory;
    };
}

#pragma warning(pop)
