#pragma once

#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext3
{
    struct BufferDbvar : public Dod::DTable<int32_t>
    {

    };

    struct CBufferDbvar : public Dod::ImTable<int32_t>
    {
        CBufferDbvar(const BufferDbvar& mutTable)
        {
            *this = mutTable;
        }
    };

    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        BufferDbvar dbvar;
    };

    struct CData
    {

        CBufferDbvar dbvar;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { CBufferDbvar(context.dbvar) };
    }

}

namespace Game::Context
{
}

#pragma warning(pop)
