struct Data
{
    void load() noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    int32_t var1{};
    float var2{};
    Types::Data::CustomData1 var3;
    Dod::MemPool memory;
    Dod::DBBuffer<float> dbvar1;
    Dod::DBBuffer<int64_t> dbvar2;
};

struct CData
{

    Dod::ImBuffer<float> dbvar1;
    Dod::ImBuffer<int64_t> dbvar2;
};
