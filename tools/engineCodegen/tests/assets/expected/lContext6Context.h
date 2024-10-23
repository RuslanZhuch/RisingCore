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

    struct MutBufferData1 : public Dod::MutTable<int32_t, float>
    {
        MutBufferData1() = default;
        MutBufferData1(const BufferData1& dTable)
        {
            this->dataBegin = dTable.dataBegin;
            this->capacityEls = dTable.capacityEls;
            this->numOfElements = dTable.numOfFilledEls;
        }
    };

    struct MutBufferData2 : public Dod::MutTable<Types::Cats::CryingCat, float, int64_t>
    {
        MutBufferData2() = default;
        MutBufferData2(const BufferData2& dTable)
        {
            this->dataBegin = dTable.dataBegin;
            this->capacityEls = dTable.capacityEls;
            this->numOfElements = dTable.numOfFilledEls;
        }
    };

    struct MutBufferData3 : public Dod::MutTable<float>
    {
        MutBufferData3() = default;
        MutBufferData3(const BufferData3& dTable)
        {
            this->dataBegin = dTable.dataBegin;
            this->capacityEls = dTable.capacityEls;
            this->numOfElements = dTable.numOfFilledEls;
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

    struct CBufferData1 : public Dod::ImTable<int32_t, float>
    {
        CBufferData1() = default;
        CBufferData1(const BufferData1& mutTable)
        {
            this->dataBegin = mutTable.dataBegin;
            this->capacityEls = mutTable.capacityEls;
            this->numOfElements = mutTable.numOfFilledEls;
        }
    };

    struct CBufferData2 : public Dod::ImTable<Types::Cats::CryingCat, float, int64_t>
    {
        CBufferData2() = default;
        CBufferData2(const BufferData2& mutTable)
        {
            this->dataBegin = mutTable.dataBegin;
            this->capacityEls = mutTable.capacityEls;
            this->numOfElements = mutTable.numOfFilledEls;
        }
    };

    struct CBufferData3 : public Dod::ImTable<float>
    {
        CBufferData3() = default;
        CBufferData3(const BufferData3& mutTable)
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
        return { CBufferDbvar2(context.dbvar2), CBufferData1(context.data1), CBufferData2(context.data2), CBufferData3(context.data3) };
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

    [[nodiscard]] static auto decoupleData(LContext6::MutBufferData1& data1ToDecouple) noexcept
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

    [[nodiscard]] static auto decoupleData(LContext6::MutBufferData2& data2ToDecouple) noexcept
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
