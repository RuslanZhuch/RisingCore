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

