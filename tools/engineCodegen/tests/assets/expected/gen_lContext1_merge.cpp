
void Data::merge([[maybe_unused]] const Data& other) noexcept
{
    this->var1 += other.var1;
    this->var2 = other.var2;
    Dod::BufferUtils::append(this->dbvar1, Dod::BufferUtils::createImFromBuffer(other.dbvar1));
    Dod::BufferUtils::append(this->dbvar2, Dod::BufferUtils::createImFromBuffer(other.dbvar2));
}
