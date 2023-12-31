#include "SContext2Context.h"

#include <engine/contextUtils.h>

namespace Game::Context::SContext2
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/sContext2.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->var, loadingDataArray, 0);


    }

    void Data::reset() noexcept
    {
    }

    void Data::merge([[maybe_unused]] const Data& other) noexcept
    {
    }

}
