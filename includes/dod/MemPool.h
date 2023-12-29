#pragma once

#include "MemTypes.h"


#pragma warning(push)
#pragma warning(disable : 4365)

#include <vector>
#pragma warning(pop)

namespace Dod
{

	struct MemPool
	{

		void allocate(MemTypes::capacity_t capacityInBytes) noexcept;

		MemTypes::dataPoint_t dataBegin{ nullptr };
		MemTypes::dataPoint_t dataEnd{ nullptr };

	private:
		std::vector<MemTypes::data_t> data;
		
	};

};
