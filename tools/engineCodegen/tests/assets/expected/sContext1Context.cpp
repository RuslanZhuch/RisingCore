#include "SContext1Context.h"

#include <engine/contextUtils.h>

namespace Game::Context::SContext1
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/sContext1.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 0) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };




    }

    void Data::reset() noexcept
    {
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
    }

}
