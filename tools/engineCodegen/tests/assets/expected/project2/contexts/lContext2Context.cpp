#include "LContext2Context.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::LContext2
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/project2/contexts/lContext2.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->var1, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->var2, loadingDataArray, 1);

        const auto dbvar0Capacity{ Engine::ContextUtils::getBufferCapacity<float>(loadingDataArray, 2) };
        const auto dbvar1Capacity{ Engine::ContextUtils::getBufferCapacity<int64_t>(loadingDataArray, 3) };

        int32_t needBytes{};
        needBytes += dbvar0Capacity.numOfBytes;
        needBytes += dbvar1Capacity.numOfBytes;

        this->memory.allocate(needBytes);
        Dod::MemTypes::capacity_t header{};

        Engine::ContextUtils::initData(this->dbvar0, dbvar0Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->dbvar0, loadingDataArray, 2);
        Engine::ContextUtils::initData(this->dbvar1, dbvar1Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->dbvar1, loadingDataArray, 3);

    }

    void Data::reset() noexcept
    {
        Dod::DataUtils::flush(this->dbvar0);
        Dod::DataUtils::flush(this->dbvar1);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        Dod::DataUtils::append(this->dbvar0, Dod::DataUtils::createImFromBuffer(other.dbvar0));
        Dod::DataUtils::append(this->dbvar1, Dod::DataUtils::createImFromBuffer(other.dbvar1));
    }

}
