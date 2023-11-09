#pragma once

#include <Contexts/LContext1Context.h>
#include <Contexts/LContext2Context.h>
#include <Contexts/LContext3Context.h>
#include <Contexts/SContext1Context.h>
#include <Contexts/SContext2Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)

namespace Game::ExecutionBlock
{
    class Executor2
    {
    public:
        void loadContext() noexcept;
        void initiate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;
        void modifyTarget1(Context::LContext1::Data&) noexcept;
        void modifyTarget2(Context::LContext3::Data&) noexcept;
        void modifyTarget3(Context::LContext3::Data&) noexcept;

    private:
        void initImpl() noexcept;
        void updateImpl(float dt) noexcept;

    public:
        Dod::MemPool memory;
        Context::LContext1::Data local1Context;
        Context::LContext2::Data local2Context;
        Context::LContext2::Data local3Context;
        Context::SContext1::CData shared1Context;
        Context::SContext1::CData shared2Context;
        Context::SContext2::CData shared3Context;
        Context::SContext2::CData shared4Context;
    };
}

#pragma warning(pop)
