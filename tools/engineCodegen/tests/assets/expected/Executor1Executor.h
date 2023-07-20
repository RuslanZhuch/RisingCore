#pragma once


#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Executor1
    {
    public:
        void loadContext() noexcept;
        void initiate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;

    private:
        void initImpl() noexcept;
        void updateImpl(float dt) noexcept;

    public:
        Dod::MemPool memory;
    };
}
