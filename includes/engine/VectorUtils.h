#include <type_traits>
#include <concepts>
#include <memory>

#include "ContainersCommon.h"

namespace Engine::ContainerUtils
{

	template <typename VecType>
	concept VectorWithData = requires(VecType t) {
		t.data;
		requires IsCStyleArray<decltype(t.data)>;
	};

	template <typename VecType>
	concept VectorIteratable = ContainerWithSize<VecType> && VectorWithData<VecType>;

	template <typename VecType>
	concept Vector = ContainerWithSize<VecType> && ContainerWithCapacity<VecType> && VectorWithData<VecType>;

	template <Vector VecType, typename PushType>
	requires std::is_same_v<typename std::decay_t<VecType>::type_t, std::decay_t<PushType>>
	void pushBack(VecType& vector, PushType pushElement) requires requires { requires sizeof(PushType) <= sizeof(void*); }
	{
		if (getNumOfElements(vector) < getCapacity(vector))
			vector.data[vector.size++] = pushElement;
	}

	template <Vector VecType, typename PushType>
	requires std::is_same_v<typename std::decay_t<VecType>::type_t, std::decay_t<PushType>>
	void pushBack(VecType& vector, const PushType& pushElement) requires requires { requires sizeof(PushType) > sizeof(void*); }
	{
		if (getNumOfElements(vector) < getCapacity(vector))
			vector.data[vector.size++] = pushElement;
	}

	template <VectorIteratable VecType>
	void forEach(VecType&& vector, std::invocable<typename std::decay_t<VecType>::type_t> auto callback)
	{
		for (int32_t elId{}; elId < vector.size; ++elId)
			callback(vector.data[elId]);
	}

	void clear(ContainerWithSize auto& vector)
	{
		vector.size = 0;
	}

	template <Vector DestVec, Vector SrcVec>
	requires std::is_same_v<typename std::decay_t<DestVec>::type_t, typename std::decay_t<SrcVec>::type_t>
	void assign(DestVec& dst, SrcVec&& src)
	{
		using type_t = typename DestVec::type_t;
		const auto totalElements{ dst.cap < src.size ? dst.cap : src.size };
		std::memcpy(dst.data, src.data, totalElements * sizeof(type_t));
		dst.size = totalElements;
	}

}