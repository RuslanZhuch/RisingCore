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

