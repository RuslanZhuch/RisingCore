#pragma once

#include <engine/types.h>

namespace Types::Group1
{

    struct Type1
    {
        ENGINE_TYPES_DESERIALIZE;

        int x{};
        float y{};
    }

    struct Type2
    {
        ENGINE_TYPES_DESERIALIZE;

        double vel{};
    }

}
