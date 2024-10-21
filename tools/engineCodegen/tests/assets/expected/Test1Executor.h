#pragma once

#include <Contexts/Type1Context.h>
#include <Contexts/Type2Context.h>
#include <Contexts/Type3Context.h>
#include <Contexts/Type4Context.h>
#include <Contexts/Type5Context.h>
#include <Contexts/Type6Context.h>
#include <Contexts/Type7Context.h>
#include <Contexts/Type8Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)

namespace Game::ExecutionBlock
{
    class Test1
    {
    public:
        struct ExternalContexts
        {
            Context::Type7::Data external1Context;
            Context::Type7::Data external2Context;
            Context::Type8::Data external3Context;
        };

        struct CExternalContexts
        {
            Context::Type7::CData external1Context;
            Context::Type7::CData external2Context;
            Context::Type8::CData external3Context;
        };

    public:
        void loadContext() noexcept;
        void initiate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;
        void modifyTarget1(Context::Type4::Data&) noexcept;
        void modifyTarget2(Context::Type4::Data&) noexcept;
        void modifyTarget3(Context::Type4::Data&) noexcept;
        [[nodiscard]] CExternalContexts getExternalContext() noexcept;

    private:
        void initImpl(ExternalContexts& externalContexts) noexcept;
        void updateImpl(ExternalContexts& externalContexts, float dt) noexcept;

        void modifyTarget1(const CExternalContexts& externalContexts, Context::Type4::Data&) noexcept;
        void modifyTarget2(const CExternalContexts& externalContexts, Context::Type4::Data&) noexcept;
        void modifyTarget3(const CExternalContexts& externalContexts, Context::Type4::Data&) noexcept;

    public:
        Dod::MemPool memory;
        Context::Type1::Data local1Context;
        Context::Type2::Data local2Context;
        Context::Type3::Data local3Context;
        Context::Type3::Data local4Context;
        Context::Type5::CData shared1Context;
        Context::Type5::CData shared2Context;
        Context::Type6::CData shared3Context;
    };
}

#pragma warning(pop)
