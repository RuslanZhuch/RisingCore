struct Data
{
    void load() noexcept;
    void reset() noexcept;
    void merge(const Data& other) noexcept;

    Dod::MemPool memory;
    BufferDbvar2 dbvar2;
    BufferData1 data1;
    BufferData2 data2;
    BufferData3 data3;
};

struct CData
{

    CBufferDbvar2 dbvar2;
    CBufferData1 data1;
    CBufferData2 data2;
    CBufferData3 data3;
};
