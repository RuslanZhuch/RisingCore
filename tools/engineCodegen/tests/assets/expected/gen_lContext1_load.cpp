
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

    const auto dbvar1CapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 3) };
    const auto dbvar2CapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int64_t>(loadingDataArray, 4) };

    int32_t needBytes{};
    needBytes += dbvar1CapacityBytes;
    needBytes += dbvar2CapacityBytes;

    this->memory.allocate(needBytes);
    int32_t header{};

    Engine::ContextUtils::loadBuffer(this->dbvar1, dbvar1CapacityBytes, this->memory, header);
    Engine::ContextUtils::loadBuffer(this->dbvar2, dbvar2CapacityBytes, this->memory, header);

}
