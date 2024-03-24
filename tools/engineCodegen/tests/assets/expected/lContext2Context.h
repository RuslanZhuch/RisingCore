#pragma once

#include <assets/types/cpp/cats/Crying.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext2
{
    struct BufferDbvar0 : public Dod::DTable<float>
    {

    };

    struct BufferDbvar1 : public Dod::DTable<int64_t>
    {

    };

    struct CBufferDbvar0 : public Dod::ImTable<float>
    {

    };

    struct CBufferDbvar1 : public Dod::ImTable<int64_t>
    {

    };

    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        int32_t var1{};
        Types::Cats::CryingCat cat;
        float var2{};
        Dod::MemPool memory;
        BufferDbvar0 dbvar0;
        BufferDbvar1 dbvar1;
    };

    struct CData
    {

        CBufferDbvar0 dbvar0;
        CBufferDbvar1 dbvar1;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { Dod::ImTable<float>(context.dbvar0), Dod::ImTable<int64_t>(context.dbvar1) };
    }

}

namespace Game::Context
{
}

#pragma warning(pop)
