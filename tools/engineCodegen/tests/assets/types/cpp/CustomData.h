#pragma once

#include <engine/types.h>

namespace Types::Data
{

    struct CustomData1
    {
        ENGINE_TYPES_DESERIALIZE;

        float dataFloat{};
        int dataInt{};
        Engine::String dataString{};
    }

}
