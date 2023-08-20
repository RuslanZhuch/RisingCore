#pragma once

#include <engine/contextUtils.h>

namespace Types
{

    static void setVariableByName(Types::ComplexType& type, std::string_view name, auto newValue) noexcept
    {
        if (name == "int1")
        {
            Engine::ContextUtils::loadVariable(type.int1, = newValue;
            return;
        }
        if (name == "float1")
        {
            type.float1 = newValue;
            return;
        }
        if (name == "string1")
        {
            type.string1 = newValue;
            return;
        }
        if (name == "innerComplex1")
        {
            type.innerComplex1 = newValue;
            return;
        }
        const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/contexts/lContext6Schema.json") };
    }

    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        ComplexType var1;
        Dod::MemPool memory;
    };
}
