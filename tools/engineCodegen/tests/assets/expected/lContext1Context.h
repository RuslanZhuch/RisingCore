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

    struct MutBufferDbvar1 : public Dod::MutTable<float>
    {
        MutBufferDbvar1() = default;
        MutBufferDbvar1(const BufferDbvar1& dTable)
        {
            this->dataBegin = dTable.dataBegin;
            this->capacityEls = dTable.capacityEls;
            this->numOfElements = dTable.numOfFilledEls;
        }
    };

    struct MutBufferDbvar2 : public Dod::MutTable<int64_t>
    {
        MutBufferDbvar2() = default;
        MutBufferDbvar2(const BufferDbvar2& dTable)
        {
            this->dataBegin = dTable.dataBegin;
            this->capacityEls = dTable.capacityEls;
            this->numOfElements = dTable.numOfFilledEls;
        }
    };

    struct CBufferDbvar1 : public Dod::ImTable<float>
    {
        CBufferDbvar1() = default;
        CBufferDbvar1(const BufferDbvar1& mutTable)
        {
            this->dataBegin = mutTable.dataBegin;
            this->capacityEls = mutTable.capacityEls;
            this->numOfElements = mutTable.numOfFilledEls;
        }
    };

    struct CBufferDbvar2 : public Dod::ImTable<int64_t>
    {
        CBufferDbvar2() = default;
        CBufferDbvar2(const BufferDbvar2& mutTable)
        {
            this->dataBegin = mutTable.dataBegin;
            this->capacityEls = mutTable.capacityEls;
            this->numOfElements = mutTable.numOfFilledEls;
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
