#pragma once

#include <assets/types/cpp/CustomData.h>
#include <dod/Buffers.h>
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
}

#pragma warning(pop)
