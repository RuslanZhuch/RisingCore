
void Data::merge([[maybe_unused]] const Data& other) noexcept
{
    Dod::DataUtils::append(this->dbvar2, Dod::DataUtils::createImFromBuffer(other.dbvar2));
    Dod::DataUtils::append(this->data1, other.data1);
    Dod::DataUtils::append(this->data2, other.data2);
}
