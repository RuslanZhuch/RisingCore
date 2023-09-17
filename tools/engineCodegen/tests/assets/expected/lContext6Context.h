#pragma once

#include <dod/Buffers.h>
#include <dod/Tables.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext6
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        Dod::DBTable<int32_t, double> data1;
    };
}

#pragma warning(pop)
