class Test2
{
public:
    [[nodiscard]] float publicMethod1() const noexcept;
    void publicMethod2(float dt) noexcept;

private:
    [[nodiscard]] int privateMethod1(int n) const noexcept;

public:
    int var1{};
    Dod::MemPool memory;
};
