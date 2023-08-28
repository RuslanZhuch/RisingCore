#pragma once

#include "MemTypes.h"
#include "CommonData.h"

#pragma warning(push)
#pragma warning(disable : 4365)
#include <vector>
#pragma warning(pop)

namespace Dod
{

	template <typename T>
	struct DBBuffer : CommonData::Buffer
	{
		using type_t = typename T;
		T* dataBegin{ nullptr };
		T* dataEnd{ nullptr };
		int32_t pad[3];
		int32_t numOfFilledEls{ 0 };
	};

	template <typename T>
	struct ImBuffer : CommonData::Buffer
	{
		using type_t = typename T;
		const T* dataBegin{ nullptr };
		const T* dataEnd{ nullptr };
	};

	template <typename T>
	struct MutBuffer : CommonData::Buffer
	{
		using type_t = typename T;
		T* dataBegin{ nullptr };
		T* dataEnd{ nullptr };
	};

	template <typename T>
	struct SortedImBuffer : CommonData::Buffer
	{
		using type_t = typename T;
		const T* dataBegin{ nullptr };
		const T* dataEnd{ nullptr };
		const int32_t* sortingDataBegin{ nullptr };
		const int32_t* sortingDataEnd{ nullptr };
	};

};
