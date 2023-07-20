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

        const auto dbvar0CapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 2) };
        const auto dbvar1CapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int64_t>(loadingDataArray, 3) };

        int32_t needBytes{};
        needBytes += dbvar0CapacityBytes;
        needBytes += dbvar1CapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->dbvar0, dbvar0CapacityBytes, this->memory, header);
        Engine::ContextUtils::loadBuffer(this->dbvar1, dbvar1CapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->dbvar0);
        Dod::BufferUtils::flush(this->dbvar1);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->dbvar0, Dod::BufferUtils::createImFromBuffer(other.dbvar0));
        Dod::BufferUtils::append(this->dbvar1, Dod::BufferUtils::createImFromBuffer(other.dbvar1));
    }

}
