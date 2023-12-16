#include "LContext1Context.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::LContext1
{

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

        const auto dbvar1Capacity{ Engine::ContextUtils::getBufferCapacity<float>(loadingDataArray, 3) };
        const auto dbvar2Capacity{ Engine::ContextUtils::getBufferCapacity<int64_t>(loadingDataArray, 4) };

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

    void Data::reset() noexcept
    {
        Dod::DataUtils::flush(this->dbvar1);
        Dod::DataUtils::flush(this->dbvar2);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        this->var1 += other.var1;
        this->var2 = other.var2;
        Dod::DataUtils::append(this->dbvar1, Dod::DataUtils::createImFromBuffer(other.dbvar1));
        Dod::DataUtils::append(this->dbvar2, Dod::DataUtils::createImFromBuffer(other.dbvar2));
    }

}
