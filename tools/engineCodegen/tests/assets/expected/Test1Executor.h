#pragma once

#include <Contexts/Type1Context.h>
#include <Contexts/Type2Context.h>
#include <Contexts/Type3Context.h>
#include <Contexts/Type4Context.h>
#include <Contexts/Type5Context.h>
#include <Contexts/Type6Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Test1
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
        Context::Type1::Data local1Context;
        Context::Type2::Data local2Context;
        Context::Type3::Data local3Context;
        Context::Type3::Data local4Context;
        Context::Type4::Data target1Context;
        Context::Type4::Data target2Context;
        Context::Type4::Data target3Context;
        const Dod::SharedContext::Controller<Context::Type5::Data>* shared1Context{ nullptr };
        const Dod::SharedContext::Controller<Context::Type5::Data>* shared2Context{ nullptr };
        const Dod::SharedContext::Controller<Context::Type6::Data>* shared3Context{ nullptr };
    };
}
