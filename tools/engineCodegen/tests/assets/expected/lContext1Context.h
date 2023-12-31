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
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        int32_t var1{};
        float var2{};
        Types::Data::CustomData1 var3;
        Dod::MemPool memory;
        Dod::DBBuffer<float> dbvar1;
        Dod::DBBuffer<int64_t> dbvar2;
    };

    struct CData
    {

        Dod::ImBuffer<float> dbvar1;
        Dod::ImBuffer<int64_t> dbvar2;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { Dod::DataUtils::createImFromBuffer(context.dbvar1), Dod::DataUtils::createImFromBuffer(context.dbvar2) };
    }

}

#pragma warning(pop)
