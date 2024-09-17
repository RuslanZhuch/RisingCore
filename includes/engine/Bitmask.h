#pragma once

#include <cinttypes>

namespace Engine::Bitmask
{
    template <size_t numOfBlocks>
    struct Bitmask
    {
        uint64_t mask[numOfBlocks]{};
    };

    template <size_t numOfBlocks>
    void set(Bitmask<numOfBlocks>& bitmask, int32_t bitId)
    {
        const auto batchId{ bitId / 64 };
        const auto localIndex{ bitId % 64 };
        bitmask.mask[batchId] |= (size_t{ 1 } << localIndex);
    }

    template <size_t numOfBlocks>
    void set(Bitmask<numOfBlocks>& bitmask, int32_t bitId, bool isHigh)
    {
        const auto batchId{ bitId / 64 };
        const auto localIndex{ bitId % 64 };
        if (isHigh)
            set(bitmask, bitId);
        else
            bitmask.mask[batchId] &= ~(size_t{ 1 } << localIndex);
    }

    template <size_t numOfBlocks>
    [[nodiscard]] bool get(const Bitmask<numOfBlocks>& bitmask, int32_t bitId)
    {
        const auto batchId{ bitId / 64 };
        const auto localIndex{ bitId % 64 };
        return (bitmask.mask[batchId] & (size_t{ 1 } << localIndex)) != 0;
    }

    template <size_t numOfBlocks>
    [[nodiscard]] bool includes(const Bitmask<numOfBlocks>& outer, const Bitmask<numOfBlocks>& inner)
    {
        for (size_t blockId{}; blockId < numOfBlocks; ++blockId)
        {
            if ((outer.mask[blockId] & inner.mask[blockId]) != inner.mask[blockId])
                return false;
        }
        return true;
    }

}
