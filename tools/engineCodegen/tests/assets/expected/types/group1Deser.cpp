#include <engine/contextUtils.h>
#include <engine/String.h>

#include <types/group1Types.h>

void Types::Group1::Type1::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
    if (name == "x")
    {
        Engine::ContextUtils::assignToVariable(this->x, value);
        return;
    }
    if (name == "y")
    {
        Engine::ContextUtils::assignToVariable(this->y, value);
        return;
    }
}

void Types::Group1::Type2::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
    if (name == "vel")
    {
        Engine::ContextUtils::assignToVariable(this->vel, value);
        return;
    }
}

