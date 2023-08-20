#include <engine/contextUtils.h>
#include <engine/String.h>

#include <Type1.h>

void External::Type1::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
    if (name == "dataFloat")
    {
        Engine::ContextUtils::assignToVariable(this->dataFloat, value);
        return;
    }
    if (name == "dataInt")
    {
        Engine::ContextUtils::assignToVariable(this->dataInt, value);
        return;
    }
    if (name == "dataString")
    {
        Engine::ContextUtils::assignToVariable(this->dataString, value);
        return;
    }
}

