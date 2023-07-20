#pragma once

#include <Contexts/LContext1Context.h>
#include <Contexts/LContext2Context.h>
#include <Contexts/LContext3Context.h>
#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Executor2
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
        Context::LContext1::Data local1Context;
        Context::LContext2::Data local2Context;
        Context::LContext2::Data local3Context;
        Context::LContext1::Data target1Context;
        Context::LContext3::Data target2Context;
        Context::LContext3::Data target3Context;
        const Dod::SharedContext::Controller<Context::SContext1::Data>* shared1Context{ nullptr };
        const Dod::SharedContext::Controller<Context::SContext1::Data>* shared2Context{ nullptr };
        const Dod::SharedContext::Controller<Context::SContext2::Data>* shared3Context{ nullptr };
        const Dod::SharedContext::Controller<Context::SContext2::Data>* shared4Context{ nullptr };
    };
}
