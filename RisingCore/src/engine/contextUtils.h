#pragma once

#include <dod/MemPool.h>
#include <dod/MemUtils.h>
#include <dod/Buffers.h>
#include <dod/BufferUtils.h>

#include <engine/StringUtils.h>

#include <rapidjson/document.h>

#include <string_view>
#include <optional>

namespace Engine::ContextUtils
{
	
	[[nodiscard]] rapidjson::Document loadFileDataRoot(const std::string_view filename) noexcept;
	[[nodiscard]] std::optional<rapidjson::GenericArray<true, rapidjson::Value>> gatherContextData(const rapidjson::Document& doc, size_t numOfExpectedElements) noexcept;
	
	[[nodiscard]] static void loadVariable(auto& dest, rapidjson::GenericArray<true, rapidjson::Value> src, size_t id) noexcept
	{

        using type_t = std::decay_t<decltype(dest)>;

        if (!src[id].IsObject())
            return;

        const auto& dataObject{ src[id].GetObject() };

        if constexpr (std::is_same_v<type_t, int32_t>)
            dest = dataObject["initial"].GetInt();
        else if constexpr (std::is_same_v<type_t, uint32_t>)
            dest = dataObject["initial"].GetUint();
        else if constexpr (std::is_same_v<type_t, float>)
            dest = dataObject["initial"].GetFloat();
        else if constexpr (requires { dest.capacity; dest.data.data(); })
            Engine::StringUtils::assign(dest, dataObject["initial"].GetString());
        
	}

    template <typename T>
    [[nodiscard]] static int32_t getBufferCapacityBytes(
        rapidjson::GenericArray<true, rapidjson::Value> buffer,
        size_t id
    ) noexcept
    {

        using type_t = T;

        if (!buffer[id].IsObject())
            return {};

        const auto& dataObject{ buffer[id].GetObject() };

        constexpr auto dataTypeSize{ sizeof(type_t) };
        const auto capacity{ dataObject["capacity"].GetInt() };
        const auto capacityBytes{ capacity * dataTypeSize };

        return capacityBytes;

    }

    template <typename T>
    [[nodiscard]] static void loadBuffer(
        Dod::DBBuffer<T>& dest,
        int32_t capacityBytes,
        Dod::MemPool& pool, 
        int32_t& header
    ) noexcept
    {

        Dod::BufferUtils::initFromMemory(dest, Dod::MemUtils::stackAquire(pool, capacityBytes, header));

    }

}
