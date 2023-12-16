struct Data
{
    void load() noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    Dod::MemPool memory;
    Dod::DBBuffer<int64_t> dbvar2;
    Dod::DTable<int32_t, float> data1;
    Dod::DTable<Types::Cats::CryingCat, float, int64_t> data2;
    Dod::DTable<float> data3;
};

struct CData
{

    Dod::ImBuffer<int64_t> dbvar2;
    Dod::ImTable<int32_t, float> data1;
    Dod::ImTable<Types::Cats::CryingCat, float, int64_t> data2;
    Dod::ImTable<float> data3;
};
