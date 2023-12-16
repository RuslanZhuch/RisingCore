
void Data::merge([[maybe_unused]] const Data& other) noexcept
{
    this->var1 += other.var1;
    this->var2 = other.var2;
    Dod::DataUtils::append(this->dbvar1, Dod::DataUtils::createImFromBuffer(other.dbvar1));
    Dod::DataUtils::append(this->dbvar2, Dod::DataUtils::createImFromBuffer(other.dbvar2));
}
