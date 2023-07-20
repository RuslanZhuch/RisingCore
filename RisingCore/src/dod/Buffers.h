#pragma once

#include "MemTypes.h"
#include <vector>

namespace Dod
{

	template <typename T>
	struct DBBuffer
	{
		using type_t = typename T;
		T* dataBegin{ nullptr };
		T* dataEnd{ nullptr };
		int32_t pad[3];
		int32_t numOfFilledEls{ 0 };
	};

	template <typename T>
	struct ImBuffer
	{
		using type_t = typename T;
		const T* dataBegin{ nullptr };
		const T* dataEnd{ nullptr };
	};

	template <typename T>
	struct MutBuffer
	{
		using type_t = typename T;
		T* dataBegin{ nullptr };
		T* dataEnd{ nullptr };
	};

};
