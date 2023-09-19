#pragma once

#include <assets/types/cpp/cats/Crying.h>
#include <dod/Buffers.h>
#include <dod/Tables.h>
#include <dod/TableUtils.h>
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
        Dod::DBBuffer<int64_t> dbvar2;
        Dod::DBTable<int32_t, float> data1;
        Dod::DBTable<Types::Cats::CryingCat, float, int64_t> data2;
    };

    [[nodiscard]] static auto getData1(Data& context) noexcept
    {
        struct Output
        {

            Dod::MutBuffer<int32_t> values1;
            Dod::MutBuffer<float> values2;
        };
        const auto data{ Dod::DataUtils::get(context.data1) };
        return Output(std::get<0>(data), std::get<1>(data));
    }

    [[nodiscard]] static auto getData2(Data& context) noexcept
    {
        struct Output
        {

            Dod::MutBuffer<Types::Cats::CryingCat> values3;
            Dod::MutBuffer<float> values4;
            Dod::MutBuffer<int64_t> values5;
        };
        const auto data{ Dod::DataUtils::get(context.data2) };
        return Output(std::get<0>(data), std::get<1>(data), std::get<2>(data));
    }

    static void addData1(Data& context, int32_t values1, float values2, bool bStrobe = true) noexcept
    {
        Dod::DataUtils::populate(context.data1, bStrobe, values1, values2);
    }

    static void addData2(Data& context, Types::Cats::CryingCat values3, float values4, int64_t values5, bool bStrobe = true) noexcept
    {
        Dod::DataUtils::populate(context.data2, bStrobe, values3, values4, values5);
    }

}

#pragma warning(pop)
