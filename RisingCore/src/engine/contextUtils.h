#pragma once

#include <dod/MemPool.h>
#include <dod/MemUtils.h>
#include <dod/Buffers.h>
#include <dod/DataUtils.h>

#include <engine/StringUtils.h>

#include <engine/jsonParser.h>

#pragma warning(push)

#pragma warning(disable : 4464)
#pragma warning(disable : 4820)
#pragma warning(disable : 4061)
#pragma warning(disable : 4365)

#include <string_view>
#include <optional>

#pragma warning(pop)

namespace Engine::ContextUtils
{
	
	[[nodiscard]] rapidjson::Document loadFileDataRoot(const std::string_view filename) noexcept;
	[[nodiscard]] std::optional<rapidjson::GenericArray<true, rapidjson::Value>> gatherContextData(const rapidjson::Document& doc, size_t numOfExpectedElements) noexcept;

    void assignToVariable(auto& dest, const rapidjson::Value& src)
    {

        using type_t = std::decay_t<decltype(dest)>;

        if (src.IsObject()) 
        {
            const auto& mapVariable{ src.GetObject() };
            if constexpr (requires(decltype(dest) d) { 
                {d.setValueByName(std::declval<std::string_view>(), std::declval<rapidjson::Value>())} -> std::convertible_to<void>;
            })
            {
                for (const auto& variable : mapVariable)
                {
                    dest.setValueByName(variable.name.GetString(), variable.value);
                }
            }
        }
        else
        {
            if constexpr (std::is_same_v<type_t, int32_t>)
                dest = src.GetInt();
            else if constexpr (std::is_same_v<type_t, uint32_t>)
                dest = src.GetUint();
            else if constexpr (std::is_same_v<type_t, float>)
                dest = src.GetFloat();
            else if constexpr (requires { dest.capacity; dest.internalData.data(); })
                Engine::StringUtils::assign(dest, src.GetString());
        }

    }

	[[nodiscard]] static void loadVariable(auto& dest, rapidjson::GenericArray<true, rapidjson::Value> src, rapidjson::SizeType id) noexcept
	{

        if (!src[id].IsObject())
            return;

        const auto& dataObject{ src[id].GetObject() };

        if (dataObject.FindMember("initial") == dataObject.end())
            return;

        assignToVariable(dest, dataObject["initial"]);

	}

    [[nodiscard]] static void loadVariableFromList(auto& dest, rapidjson::GenericArray<true, rapidjson::Value> src, int32_t id) noexcept
    {

        assignToVariable(dest, src[static_cast<rapidjson::SizeType>(id)]);

    }

    struct CapacityData
    {
        Dod::MemTypes::capacity_t numOfBytes{};
        int32_t numOfElements{};
        int32_t pad{};
    };

    template <typename T>
    [[nodiscard]] static auto getBufferCapacity(
        rapidjson::GenericArray<true, rapidjson::Value> buffer,
        rapidjson::SizeType id
    ) noexcept
    {

        using type_t = T;

        CapacityData data;

        if (!buffer[id].IsObject())
            return data;

        const auto& dataObject{ buffer[id].GetObject() };

        constexpr auto dataTypeSize{ sizeof(type_t) };
        const auto numOfElements{ dataObject["capacity"].GetInt() };
        const auto capacityBytes{ numOfElements * dataTypeSize };

        data.numOfBytes = capacityBytes;
        data.numOfElements = numOfElements;
        return data;

    }

    template <typename ... Types>
    [[nodiscard]] static auto getDataCapacity(
        rapidjson::GenericArray<true, rapidjson::Value> buffer,
        rapidjson::SizeType id
    ) noexcept
    {

        CapacityData data;

        if (!buffer[id].IsObject())
            return data;

        using types_t = std::tuple<Types...>;

        const auto& dataObject{ buffer[id].GetObject() };

        const auto numOfElements{ dataObject["capacity"].GetInt() };
        const auto capacityBytes{ Dod::DataUtils::computeCapacityInBytes<types_t>(numOfElements) };

        data.numOfBytes = capacityBytes;
        data.numOfElements = numOfElements;
        return data;

    }

    template <typename T>
    static void initData(
        Dod::DBBuffer<T>& dest,
        CapacityData capacityData,
        Dod::MemPool& pool, 
        Dod::MemTypes::capacity_t& header
    ) noexcept
    {

        Dod::DataUtils::initFromMemory(dest, Dod::MemUtils::stackAquire(pool, capacityData.numOfBytes, alignof(T), header));

    }

    template <typename ... Types>
    static void initData(
        Dod::CommonData::CTable auto& dest,
        CapacityData capacityData,
        Dod::MemPool& pool,
        Dod::MemTypes::capacity_t& header
    ) noexcept
    {

        constexpr auto alignment{ RisingCore::Helpers::findLargestAlignment<RisingCore::Helpers::gatherTypes<Types...>>() };
        Dod::DataUtils::initFromMemory(dest, capacityData.numOfElements, Dod::MemUtils::stackAquire(pool, capacityData.numOfBytes, alignment, header));

    }

    template <typename T>
    [[nodiscard]] static void loadDataContent(
        Dod::DBBuffer<T>& dest,
        rapidjson::GenericArray<true, rapidjson::Value> src, 
        int32_t id
    ) noexcept
    {

        if (!src[static_cast<rapidjson::SizeType>(id)].IsObject())
            return;
        const auto& dataObject{ src[static_cast<rapidjson::SizeType>(id)].GetObject() };

        const auto initialField{ dataObject.FindMember("initial") };
        if (initialField == dataObject.end())
            return;

        const auto initialData{ initialField->value.GetArray() };

        const auto numOfElementsToLoad{ static_cast<int32_t>(initialData.Size()) };
        const auto bufferSize{ Dod::DataUtils::getCapacity(dest) };

        const auto totalElements{ std::min(numOfElementsToLoad, bufferSize) };

        for (int32_t elId{}; elId < totalElements; ++elId)
        {
            Dod::DataUtils::constructBack(dest);
            loadVariableFromList(Dod::DataUtils::get(dest, elId), initialData, elId);
        }

    }

}
