#include "contextUtils.h"

#include <fstream>

[[nodiscard]] rapidjson::Document Engine::ContextUtils::loadFileDataRoot(const std::string_view filename) noexcept
{

    std::ifstream contextFile(filename.data());

    rapidjson::Document json;

    if (!contextFile.is_open())
        return json;

    const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

    json.Parse(fileRawData.c_str());
    return json;

}

[[nodiscard]] std::optional<rapidjson::GenericArray<true, rapidjson::Value>> Engine::ContextUtils::gatherContextData(const rapidjson::Document& doc, size_t numOfExpectedElements) noexcept
{

    if (!doc.IsObject())
        return {};

    for (const auto& element : doc.GetObject())
    {

        if (element.name != "data" || !element.value.IsArray())
            continue;

        const auto& arrayData{ element.value.GetArray() };
        if (arrayData.Size() == numOfExpectedElements)
            return arrayData;

    }

  return {};

}
