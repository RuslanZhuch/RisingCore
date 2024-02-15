
void Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext1.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };

    if (!inputDataOpt.has_value())
    {
        return;
    }

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(this->var1, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(this->var2, loadingDataArray, 1);
    Engine::ContextUtils::loadVariable(this->var3, loadingDataArray, 2);

    const auto dbvar1Capacity{ Engine::ContextUtils::getDataCapacity<float>(loadingDataArray, 3) };
    const auto dbvar2Capacity{ Engine::ContextUtils::getDataCapacity<int64_t>(loadingDataArray, 4) };

    int32_t needBytes{ 64 };
    needBytes += dbvar1Capacity.numOfBytes;
    needBytes += dbvar2Capacity.numOfBytes;

    this->memory.allocate(needBytes);
    Dod::MemTypes::capacity_t header{};

    Engine::ContextUtils::initData(this->dbvar1, dbvar1Capacity, this->memory, header);
    Engine::ContextUtils::loadDataContent(this->dbvar1, loadingDataArray, 3);
    Engine::ContextUtils::initData(this->dbvar2, dbvar2Capacity, this->memory, header);
    Engine::ContextUtils::loadDataContent(this->dbvar2, loadingDataArray, 4);

}
