#pragma once

#include <assets/types/cpp/cats/Crying.h>
#include <dod/Tables.h>
#include <dod/DataUtils.h>
#include <dod/MemPool.h>

#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)

namespace Game::Context::LContext6
{
    struct BufferDbvar2 : public Dod::DTable<int64_t>
    {

    };

    struct BufferData1 : public Dod::DTable<int32_t, float>
    {

    };

    struct BufferData2 : public Dod::DTable<Types::Cats::CryingCat, float, int64_t>
    {

    };

    struct BufferData3 : public Dod::DTable<float>
    {

    };

    struct CBufferDbvar2 : public Dod::ImTable<int64_t>
    {

    };

    struct CBufferData1 : public Dod::ImTable<int32_t, float>
    {

    };

    struct CBufferData2 : public Dod::ImTable<Types::Cats::CryingCat, float, int64_t>
    {

    };

    struct CBufferData3 : public Dod::ImTable<float>
    {

    };

    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        BufferDbvar2 dbvar2;
        BufferData1 data1;
        BufferData2 data2;
        BufferData3 data3;
    };

    struct CData
    {
        CBufferDbvar2 dbvar2;
        CBufferData1 data1;
        CBufferData2 data2;
        CBufferData3 data3;
    };

    [[nodiscard]] static CData convertToConst(const Data& context) noexcept
    {
        return { Dod::ImTable<int64_t>(context.dbvar2), Dod::ImTable<int32_t, float>(context.data1), Dod::ImTable<Types::Cats::CryingCat, float, int64_t>(context.data2), Dod::ImTable<float>(context.data3) };
    }

}

namespace Game::Context
{
    [[nodiscard]] static auto decoupleData(LContext6::BufferData1& data1ToDecouple) noexcept
    {
        struct Output
        {

            Dod::MutTable<int32_t> values1;
            Dod::MutTable<float> values2;
        };
        const auto data{ Dod::DataUtils::get(data1ToDecouple) };
        return Output(std::get<0>(data), std::get<1>(data));
    }

    [[nodiscard]] static auto decoupleData(const LContext6::CBufferData1& data1ToDecouple) noexcept
    {
        struct Output
        {

            Dod::ImTable<int32_t> values1;
            Dod::ImTable<float> values2;
        };
        const auto data{ Dod::DataUtils::get(data1ToDecouple) };
        return Output(std::get<0>(data), std::get<1>(data));
    }

    [[nodiscard]] static auto decoupleData(LContext6::BufferData2& data2ToDecouple) noexcept
    {
        struct Output
        {

            Dod::MutTable<Types::Cats::CryingCat> values3;
            Dod::MutTable<float> values4;
            Dod::MutTable<int64_t> values5;
        };
        const auto data{ Dod::DataUtils::get(data2ToDecouple) };
        return Output(std::get<0>(data), std::get<1>(data), std::get<2>(data));
    }

    [[nodiscard]] static auto decoupleData(const LContext6::CBufferData2& data2ToDecouple) noexcept
    {
        struct Output
        {

            Dod::ImTable<Types::Cats::CryingCat> values3;
            Dod::ImTable<float> values4;
            Dod::ImTable<int64_t> values5;
        };
        const auto data{ Dod::DataUtils::get(data2ToDecouple) };
        return Output(std::get<0>(data), std::get<1>(data), std::get<2>(data));
    }

    static void addData(LContext6::BufferData1& data1Dst, int32_t values1, float values2, bool bStrobe = true) noexcept
    {
        Dod::DataUtils::pushBack(data1Dst, bStrobe, values1, values2);
    }

    static void addData(LContext6::BufferData2& data2Dst, Types::Cats::CryingCat values3, float values4, int64_t values5, bool bStrobe = true) noexcept
    {
        Dod::DataUtils::pushBack(data2Dst, bStrobe, values3, values4, values5);
    }

}

#pragma warning(pop)
