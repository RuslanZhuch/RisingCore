#pragma once

#include <string_view>
#include <rapidjson/document.h>

#define ENGINE_TYPES_DESERIALIZE void setValueByName(std::string_view name, const rapidjson::Value& value) noexcept;
