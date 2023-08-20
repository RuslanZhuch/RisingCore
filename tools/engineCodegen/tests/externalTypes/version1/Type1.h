#pragma once

#include <engine/types.h>

namespace External
{

    struct Type1
    {
        ENGINE_TYPES_DESERIALIZE;

        float dataFloat{};
        int dataInt{};
        Engine::String dataString{};
    }

}
