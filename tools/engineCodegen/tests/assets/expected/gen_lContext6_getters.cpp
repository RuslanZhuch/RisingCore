[[nodiscard]] static auto getData1(Data& context) noexcept
{
    struct Output
    {

        Dod::MutTable<int32_t> values1;
        Dod::MutTable<float> values2;
    };
    const auto data{ Dod::DataUtils::get(context.data1) };
    return Output(std::get<0>(data), std::get<1>(data));
}

[[nodiscard]] static auto getData1(const CData& context) noexcept
{
    struct Output
    {

        Dod::ImTable<int32_t> values1;
        Dod::ImTable<float> values2;
    };
    const auto data{ Dod::DataUtils::get(context.data1) };
    return Output(std::get<0>(data), std::get<1>(data));
}

[[nodiscard]] static auto getData2(Data& context) noexcept
{
    struct Output
    {

        Dod::MutTable<Types::Cats::CryingCat> values3;
        Dod::MutTable<float> values4;
        Dod::MutTable<int64_t> values5;
    };
    const auto data{ Dod::DataUtils::get(context.data2) };
    return Output(std::get<0>(data), std::get<1>(data), std::get<2>(data));
}

[[nodiscard]] static auto getData2(const CData& context) noexcept
{
    struct Output
    {

        Dod::ImTable<Types::Cats::CryingCat> values3;
        Dod::ImTable<float> values4;
        Dod::ImTable<int64_t> values5;
    };
    const auto data{ Dod::DataUtils::get(context.data2) };
    return Output(std::get<0>(data), std::get<1>(data), std::get<2>(data));
}

