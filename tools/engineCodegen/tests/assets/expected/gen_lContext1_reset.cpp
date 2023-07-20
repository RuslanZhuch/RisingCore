
void Data::reset() noexcept
{
    Dod::BufferUtils::flush(this->dbvar1);
    Dod::BufferUtils::flush(this->dbvar2);
}
