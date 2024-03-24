static void addData(LContext6::BufferData1& data1Dst, int32_t values1, float values2, bool bStrobe = true) noexcept
{
    Dod::DataUtils::pushBack(data1Dst, bStrobe, values1, values2);
}

static void addData(LContext6::BufferData2& data2Dst, Types::Cats::CryingCat values3, float values4, int64_t values5, bool bStrobe = true) noexcept
{
    Dod::DataUtils::pushBack(data2Dst, bStrobe, values3, values4, values5);
}

