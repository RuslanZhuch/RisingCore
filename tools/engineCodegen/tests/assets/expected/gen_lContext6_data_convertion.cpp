[[nodiscard]] static CData convertToConst(const Data& context) noexcept
{
    return { Dod::ImTable<int64_t>(context.dbvar2), Dod::ImTable<int32_t, float>(context.data1), Dod::ImTable<Types::Cats::CryingCat, float, int64_t>(context.data2), Dod::ImTable<float>(context.data3) };
}
