#pragma once

#include <engine/jsonParser.h>
#include <string_view>

#define ENGINE_TYPES_DESERIALIZE void setValueByName(std::string_view name, const rapidjson::Value& value) noexcept;
