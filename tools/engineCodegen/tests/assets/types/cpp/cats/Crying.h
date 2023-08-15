#pragma once

#include <engine/types.h>

namespace Types::Cats
{

    struct CryingCat
    {
        ENGINE_TYPES_DESERIALIZE;

        int numOfLegs{};
        float distance{};
    }

    struct CryingCat2
    {
        ENGINE_TYPES_DESERIALIZE;

        Engine::String name;
    }

}
