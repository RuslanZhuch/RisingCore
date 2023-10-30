#pragma once

#include <assets/types/cpp/cats/Crying.h>
#include <dod/Buffers.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext2
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        int32_t var1{};
        Types::Cats::CryingCat cat;
        float var2{};
        Dod::MemPool memory;
        Dod::DBBuffer<float> dbvar0;
        Dod::DBBuffer<int64_t> dbvar1;
    };

    struct CData
    {

        Dod::ImBuffer<float> dbvar0;
        Dod::ImBuffer<int64_t> dbvar1;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { Dod::DataUtils::createImFromBuffer(context.dbvar0), Dod::DataUtils::createImFromBuffer(context.dbvar1) };
    }

}

#pragma warning(pop)
