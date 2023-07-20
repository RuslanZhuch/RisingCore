#pragma once

#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Executor3
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
        const Dod::SharedContext::Controller<Context::SContext1::Data>* shared1Context{ nullptr };
        const Dod::SharedContext::Controller<Context::SContext2::Data>* shared3Context{ nullptr };
    };
}
