
void Data::reset() noexcept
{
    Dod::DataUtils::flush(this->dbvar1);
    Dod::DataUtils::flush(this->dbvar2);
}
