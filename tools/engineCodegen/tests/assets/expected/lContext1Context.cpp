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

        const auto dbvar1CapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 3) };
        const auto dbvar2CapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int64_t>(loadingDataArray, 4) };

        int32_t needBytes{};
        needBytes += dbvar1CapacityBytes;
        needBytes += dbvar2CapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::initBuffer(this->dbvar1, dbvar1CapacityBytes, this->memory, header);
        Engine::ContextUtils::loadBufferContent(this->dbvar1, loadingDataArray, 3);
        Engine::ContextUtils::initBuffer(this->dbvar2, dbvar2CapacityBytes, this->memory, header);
        Engine::ContextUtils::loadBufferContent(this->dbvar2, loadingDataArray, 4);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->dbvar1);
        Dod::BufferUtils::flush(this->dbvar2);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        this->var1 += other.var1;
        this->var2 = other.var2;
        Dod::BufferUtils::append(this->dbvar1, Dod::BufferUtils::createImFromBuffer(other.dbvar1));
        Dod::BufferUtils::append(this->dbvar2, Dod::BufferUtils::createImFromBuffer(other.dbvar2));
    }

}
