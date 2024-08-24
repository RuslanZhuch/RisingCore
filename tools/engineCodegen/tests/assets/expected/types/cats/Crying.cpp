#include <engine/contextUtils.h>
#include <engine/String.h>

#include <assets/types/cpp/cats/Crying.h>

void Types::Cats::CryingCat2::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
}

void Types::Cats::CryingCat::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
    if (name == "numOfLegs")
    {
        Engine::ContextUtils::assignToVariable(this->numOfLegs, value);
        return;
    }
    if (name == "distance")
    {
        Engine::ContextUtils::assignToVariable(this->distance, value);
        return;
    }
}

