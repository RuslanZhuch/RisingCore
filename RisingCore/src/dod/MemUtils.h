#pragma once

#include "MemTypes.h"

namespace Dod::MemUtils
{

	[[nodiscard]] static auto aquire(const auto& source, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
	{

		struct Output
		{
			decltype(source.dataBegin) dataBegin{ nullptr };
			decltype(source.dataEnd) dataEnd{ nullptr };
		};

		const auto sourceCapacity{ source.dataEnd - source.dataBegin };
		const auto bCanAcquire{ 
			source.dataBegin != nullptr && 
			source.dataEnd != nullptr &&
			beginIndex < sourceCapacity && 
			endIndex <= sourceCapacity && 
			endIndex > beginIndex
		};
		Output output(source.dataBegin + beginIndex * bCanAcquire, source.dataBegin + endIndex * bCanAcquire);

		return output;

	}

	[[nodiscard]] static auto stackAquire(const auto& source, int32_t numOfBytes, int32_t& header) noexcept
	{

		struct Output
		{
			MemTypes::dataPoint_t dataBegin{ nullptr };
			MemTypes::dataPoint_t dataEnd{ nullptr };
		};
		Output output;

		const auto capacity{ source.dataEnd - source.dataBegin };
		if ((source.dataBegin == nullptr) || (source.dataEnd == nullptr) || (header >= capacity) || (numOfBytes > capacity - header))
			return output;

		output.dataBegin = source.dataBegin + header;
		output.dataEnd = source.dataBegin + header + numOfBytes;

		header += numOfBytes;

		return output;

	}

};
