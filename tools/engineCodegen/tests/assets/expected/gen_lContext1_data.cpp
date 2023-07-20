struct Data
{
    void load() noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    int32_t var1{};
    float var2{};
    CustomData1 var3;
    Dod::MemPool memory;
    Dod::DBBuffer<float> dbvar1;
    Dod::DBBuffer<int64_t> dbvar2;
};
