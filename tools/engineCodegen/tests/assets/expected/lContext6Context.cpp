#include "LContext6Context.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::LContext6
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext6.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto dbvar2Capacity{ Engine::ContextUtils::getBufferCapacity<int64_t>(loadingDataArray, 0) };
        const auto data1Capacity{ Engine::ContextUtils::getDataCapacity<int32_t, float>(loadingDataArray, 1) };
        const auto data2Capacity{ Engine::ContextUtils::getDataCapacity<Types::Cats::CryingCat, int64_t>(loadingDataArray, 2) };

        int32_t needBytes{};
        needBytes += dbvar2Capacity.numOfBytes;
        needBytes += data1Capacity.numOfBytes;
        needBytes += data2Capacity.numOfBytes;

        this->memory.allocate(needBytes);
        Dod::MemTypes::capacity_t header{};

        Engine::ContextUtils::initData(this->dbvar2, dbvar2Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->dbvar2, loadingDataArray, 0);
        Engine::ContextUtils::initData(this->data1, data1Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->data1, loadingDataArray, 1);
        Engine::ContextUtils::initData(this->data2, data2Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->data2, loadingDataArray, 2);

    }

    void Data::reset() noexcept
    {
        Dod::DataUtils::flush(this->dbvar2);
        Dod::DataUtils::flush(this->data1);
        Dod::DataUtils::flush(this->data2);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        Dod::DataUtils::append(this->dbvar2, Dod::DataUtils::createImFromBuffer(other.dbvar2));
        Dod::DataUtils::append(this->data1, other.data1);
        Dod::DataUtils::append(this->data2, other.data2);
    }

}
