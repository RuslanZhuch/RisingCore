struct Test2
{
    [[nodiscard]] float method1() const noexcept;
    void method2(float dt) noexcept;
    static void method3(int32_t arg) noexcept;

    int var1{};
    Dod::MemPool memory;
};
