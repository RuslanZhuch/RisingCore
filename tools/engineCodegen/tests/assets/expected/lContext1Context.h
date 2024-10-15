#pragma once

#include <assets/types/cpp/CustomData.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext1
{
    struct BufferDbvar1 : public Dod::DTable<float>
    {

    };

    struct BufferDbvar2 : public Dod::DTable<int64_t>
    {

    };

    struct CBufferDbvar1 : public Dod::ImTable<float>
    {
        CBufferDbvar1(const BufferDbvar1& mutTable)
        {
            *this = mutTable;
        }
    };

    struct CBufferDbvar2 : public Dod::ImTable<int64_t>
    {
        CBufferDbvar2(const BufferDbvar2& mutTable)
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
        float var2{};
        Types::Data::CustomData1 var3;
        Dod::MemPool memory;
        BufferDbvar1 dbvar1;
        BufferDbvar2 dbvar2;
    };

    struct CData
    {
        int32_t var1{};
        float var2{};
        Types::Data::CustomData1 var3;
        CBufferDbvar1 dbvar1;
        CBufferDbvar2 dbvar2;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { context.var1, context.var2, context.var3, CBufferDbvar1(context.dbvar1), CBufferDbvar2(context.dbvar2) };
    }

}

namespace Game::Context
{
}

#pragma warning(pop)
