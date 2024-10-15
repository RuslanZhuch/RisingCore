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
        CBufferDbvar0(const BufferDbvar0& mutTable)
        {
            *this = mutTable;
        }
    };

    struct CBufferDbvar1 : public Dod::ImTable<int64_t>
    {
        CBufferDbvar1(const BufferDbvar1& mutTable)
        {
            *this = mutTable;
        }
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
        int32_t var1{};
        Types::Cats::CryingCat cat;
        float var2{};
        CBufferDbvar0 dbvar0;
        CBufferDbvar1 dbvar1;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { context.var1, context.cat, context.var2, CBufferDbvar0(context.dbvar0), CBufferDbvar1(context.dbvar1) };
    }

}

namespace Game::Context
{
}

#pragma warning(pop)
