
void Data::reset() noexcept
{
    Dod::DataUtils::flush(this->dbvar2);
    Dod::DataUtils::flush(this->data1);
    Dod::DataUtils::flush(this->data2);
}
