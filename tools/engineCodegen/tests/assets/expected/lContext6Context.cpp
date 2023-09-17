#include "LContext6Context.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::LContext6
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext6.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto data1Capacity{ Engine::ContextUtils::getDataCapacity<int32_t, double>(loadingDataArray, 0) };

        int32_t needBytes{};
        needBytes += data1Capacity.numOfBytes;

        this->memory.allocate(needBytes);
        Dod::MemTypes::capacity_t header{};

        Engine::ContextUtils::initData(this->data1, data1Capacity, this->memory, header);
        Engine::ContextUtils::loadDataContent(this->data1, loadingDataArray, 0);

    }

    void Data::reset() noexcept
    {
        Dod::DataUtils::flush(this->data1);
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
        Dod::DataUtils::append(this->data1, other.data1);
    }

}
