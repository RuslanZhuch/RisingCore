#pragma once

#include <dod/Buffers.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext3
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        Dod::DBBuffer<int32_t> dbvar;
    };

    struct CData
    {

        Dod::ImBuffer<int32_t> dbvar;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { Dod::DataUtils::createImFromBuffer(context.dbvar) };
    }

}

#pragma warning(pop)
