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


        const auto dbvarCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

        int32_t needBytes{};
        needBytes += dbvarCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->dbvar, dbvarCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->dbvar);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->dbvar, Dod::BufferUtils::createImFromBuffer(other.dbvar));
    }

}
