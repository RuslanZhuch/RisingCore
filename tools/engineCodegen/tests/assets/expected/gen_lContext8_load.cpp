
void Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext8.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };

    if (!inputDataOpt.has_value())
    {
        return;
    }

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(this->var1, loadingDataArray, 1);

    const auto data1Capacity{ Engine::ContextUtils::getDataCapacity<int32_t, float>(loadingDataArray, 0) };
    const auto dbvar2Capacity{ Engine::ContextUtils::getDataCapacity<int64_t>(loadingDataArray, 2) };

    int32_t needBytes{ 64 };
    needBytes += data1Capacity.numOfBytes;
    needBytes += dbvar2Capacity.numOfBytes;

    this->memory.allocate(needBytes);
    Dod::MemTypes::capacity_t header{};

    Engine::ContextUtils::initData(this->data1, data1Capacity, this->memory, header);
    Engine::ContextUtils::loadDataContent(this->data1, loadingDataArray, 0);
    Engine::ContextUtils::initData(this->dbvar2, dbvar2Capacity, this->memory, header);
    Engine::ContextUtils::loadDataContent(this->dbvar2, loadingDataArray, 2);

}
