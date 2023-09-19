struct Data
{
    void load() noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    Dod::MemPool memory;
    Dod::DBBuffer<int64_t> dbvar2;
    Dod::DBTable<int32_t, float> data1;
    Dod::DBTable<Types::Cats::CryingCat, int64_t> data2;
};
