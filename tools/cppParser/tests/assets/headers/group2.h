#pragma once

#include <engine/types.h>

namespace Types::Group1
{

    struct Type3
    {
        ENGINE_TYPES_DESERIALIZE;

        Engine::StaticVector<int, 16> static1{};
        Engine::StaticVector<float, 32> static2{};
        int initiated1{ 2 };
        float initiated2{ 42.5 };
        std::string initiated3{ "str" };
    }

}
