#include <type_traits>
#include <concepts>
#include <memory>

#include "ContainersCommon.h"

namespace Engine::ContainerUtils
{

	template <typename MapType, typename PushKeyType, typename PushDataType>
	void insert(MapType& map, PushKeyType pushKey, PushDataType pushDataElement)
	{
		for (int32_t elId{}; elId < getNumOfElements(map); ++elId)
		{
			if (map.data.keys[elId] != pushKey)
				continue;
			map.data.values[elId] = std::move(pushDataElement);
			return;
		}

		if (getNumOfElements(map) < getCapacity(map))
		{
			map.data.keys[map.size] = pushKey;
			map.data.values[map.size] = std::move(pushDataElement);
			++map.size;
		}
	}

//	template <Vector VecType, typename PushType>
//	requires std::is_same_v<typename std::decay_t<VecType>::type_t, std::decay_t<PushType>>
//	void pushBack(VecType& vector, const PushType& pushElement) requires requires { requires sizeof(PushType) > sizeof(void*); }
//	{
//		if (getNumOfElements(vector) < getCapacity(vector))
//			vector.data[vector.size++] = pushElement;
//	}
//
	template <typename MapType>
	void forEach(MapType&& map, std::invocable<typename std::decay_t<MapType>::valuesType_t> auto callback)
	{
		for (int32_t elId{}; elId < map.size; ++elId)
			callback(map.data.values[elId]);
	}

	void clear(auto& vector)
	{
		vector.size = 0;
	}

	template <typename DestMap, typename SrcMap>
	void assign(DestMap& dst, SrcMap&& src)
	{
		using keyType_t = typename DestMap::keyType_t;
		using valuesType_t = typename DestMap::valuesType_t;
		const auto totalElements{ dst.cap < src.size ? dst.cap : src.size };
		std::memcpy(dst.data.keys, src.data.keys, totalElements * sizeof(keyType_t));
		std::memcpy(dst.data.values, src.data.values, totalElements * sizeof(valuesType_t));
		dst.size = totalElements;
	}

	template <typename MapType>
	auto& getOrInsert(MapType& map, auto key)
	{
		for (int32_t elId{}; elId < getNumOfElements(map); ++elId)
		{
			if (map.data.keys[elId] != key)
				continue;
			return map.data.values[elId];
		}

		if (getNumOfElements(map) < getCapacity(map))
		{
			map.data.keys[map.size] = key;
			map.data.values[map.size] = 0;

			return map.data.values[map.size++];
		}

		return map.data.values[0];
	}
	

	template <typename MapType>
	auto& get(MapType& map, auto key)
	{
		for (int32_t elId{}; elId < getNumOfElements(map); ++elId)
		{
			if (map.data.keys[elId] != key)
				continue;
			return map.data.values[elId];
		}

		return map.data.values[0];
	}

	template <typename MapType>
	int32_t getIndex(MapType& map, auto key)
	{
		for (int32_t elId{}; elId < getNumOfElements(map); ++elId)
		{
			if (map.data.keys[elId] != key)
				continue;
			return elId;
		}

		return -1;
	}

}