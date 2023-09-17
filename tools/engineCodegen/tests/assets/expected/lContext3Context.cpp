#include "LContext3Context.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::LContext3
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext3.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto dbvarCapacity{ Engine::ContextUtils::getBufferCapacity<int32_t>(loadingDataArray, 0) };

        int32_t needBytes{};
        needBytes += dbvarCapacity.numOfBytes;

        this->memory.allocate(needBytes);
        Dod::MemTypes::capacity_t header{};

        Engine::ContextUtils::initData(this->dbvar, dbvarCapacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->dbvar, loadingDataArray, 0);

    }

    void Data::reset() noexcept
    {
        Dod::DataUtils::flush(this->dbvar);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        Dod::DataUtils::append(this->dbvar, Dod::DataUtils::createImFromBuffer(other.dbvar));
    }

}
