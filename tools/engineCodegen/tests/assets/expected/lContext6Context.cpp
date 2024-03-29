#include "LContext6Context.h"

#include <engine/contextUtils.h>

namespace Game::Context::LContext6
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext6.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto dbvar2Capacity{ Engine::ContextUtils::getDataCapacity<int64_t>(loadingDataArray, 0) };
        const auto data1Capacity{ Engine::ContextUtils::getDataCapacity<int32_t, float>(loadingDataArray, 1) };
        const auto data2Capacity{ Engine::ContextUtils::getDataCapacity<Types::Cats::CryingCat, float, int64_t>(loadingDataArray, 2) };
        const auto data3Capacity{ Engine::ContextUtils::getDataCapacity<float>(loadingDataArray, 3) };

        int32_t needBytes{ 64 };
        needBytes += dbvar2Capacity.numOfBytes;
        needBytes += data1Capacity.numOfBytes;
        needBytes += data2Capacity.numOfBytes;
        needBytes += data3Capacity.numOfBytes;

        this->memory.allocate(needBytes);
        Dod::MemTypes::capacity_t header{};

        Engine::ContextUtils::initData(this->dbvar2, dbvar2Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->dbvar2, loadingDataArray, 0);
        Engine::ContextUtils::initData(this->data1, data1Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->data1, loadingDataArray, 1);
        Engine::ContextUtils::initData(this->data2, data2Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->data2, loadingDataArray, 2);
        Engine::ContextUtils::initData(this->data3, data3Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->data3, loadingDataArray, 3);

    }

    void Data::reset() noexcept
    {
        Dod::DataUtils::flush(this->dbvar2);
        Dod::DataUtils::flush(this->data1);
        Dod::DataUtils::flush(this->data2);
        Dod::DataUtils::flush(this->data3);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        Dod::DataUtils::append(this->dbvar2, Dod::ImTable(other.dbvar2));
        Dod::DataUtils::append(this->data1, Dod::ImTable(other.data1));
        Dod::DataUtils::append(this->data2, Dod::ImTable(other.data2));
        Dod::DataUtils::append(this->data3, Dod::ImTable(other.data3));
    }

}
