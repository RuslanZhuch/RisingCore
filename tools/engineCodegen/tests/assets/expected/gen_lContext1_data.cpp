struct Data
{
    void load() noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    int32_t var1{};
    float var2{};
    Types::Data::CustomData1 var3;
    Dod::MemPool memory;
    BufferDbvar1 dbvar1;
    BufferDbvar2 dbvar2;
};

struct CData
{

    CBufferDbvar1 dbvar1;
    CBufferDbvar2 dbvar2;
};
