#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::LContext2
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        int32_t var1{};
        float var2{};
        Dod::MemPool memory;
        Dod::DBBuffer<float> dbvar0;
        Dod::DBBuffer<int64_t> dbvar1;
    };
}
