static void addData1(Data& context, int32_t values1, float values2, bool bStrobe = true) noexcept
{
    Dod::DataUtils::populate(context.data1, bStrobe, values1, values2);
}

static void addData2(Data& context, Types::Cats::CryingCat values3, float values4, int64_t values5, bool bStrobe = true) noexcept
{
    Dod::DataUtils::populate(context.data2, bStrobe, values3, values4, values5);
}

