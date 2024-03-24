
void Data::merge([[maybe_unused]] const Data& other) noexcept
{
    Dod::DataUtils::append(this->dbvar2, Dod::ImTable(other.dbvar2));
    Dod::DataUtils::append(this->data1, Dod::ImTable(other.data1));
    Dod::DataUtils::append(this->data2, Dod::ImTable(other.data2));
    Dod::DataUtils::append(this->data3, Dod::ImTable(other.data3));
}
