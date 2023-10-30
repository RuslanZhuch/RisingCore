#pragma once

#include <dod/Buffers.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::SContext1
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
    };

    struct CData
    {

    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return {  };
    }

}

#pragma warning(pop)
