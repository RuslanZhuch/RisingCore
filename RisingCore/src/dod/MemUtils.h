#pragma once

#include "MemTypes.h"
#include <algorithm>

namespace Dod::MemUtils
{

	[[nodiscard]] static auto getAlignOffset(const auto* point, MemTypes::alignment_t alignment)
	{
		const auto address{ reinterpret_cast<MemTypes::alignment_t>(point) };
		const auto shifted{ static_cast<MemTypes::alignment_t>(address & (alignment - 1)) };
		if (shifted != 0)
			return alignment - shifted;
		return MemTypes::alignment_t{};
	}

	[[nodiscard]] static auto acquire(const auto& source, MemTypes::capacity_t beginIndex, MemTypes::capacity_t numOfBytes, MemTypes::alignment_t alignment) noexcept
	{

		beginIndex = std::max(MemTypes::capacity_t{}, beginIndex);
		numOfBytes = std::max(MemTypes::capacity_t{}, numOfBytes);



		struct Output
		{
			decltype(source.dataBegin) dataBegin{ nullptr };
			decltype(source.dataEnd) dataEnd{ nullptr };
		};

		const auto initialSourceBegin{ source.dataBegin + beginIndex };
		const auto alignOffset{ MemUtils::getAlignOffset(initialSourceBegin, alignment) };
		const auto alignedSourceBegin{ source.dataBegin + alignOffset };

		const auto sourceCapacity{ source.dataEnd - source.dataBegin };
		const auto bCanAcquire{ 
			source.dataBegin != nullptr &&
			source.dataEnd != nullptr &&
			beginIndex + alignOffset < sourceCapacity &&
			beginIndex + numOfBytes + alignOffset <= sourceCapacity &&
			numOfBytes > 0
		};
		Output output(source.dataBegin + (beginIndex + alignOffset) * bCanAcquire, source.dataBegin + (beginIndex + numOfBytes + alignOffset) * bCanAcquire);

		return output;

	}

	[[nodiscard]] static auto stackAquire(const auto& source, MemTypes::capacity_t numOfBytes, MemTypes::alignment_t alignment, MemTypes::capacity_t& header) noexcept
	{

		struct Output
		{
			MemTypes::dataPoint_t dataBegin{ nullptr };
			MemTypes::dataPoint_t dataEnd{ nullptr };
		};
		Output output;

		const auto initialSourceBegin{ source.dataBegin + header };
		const auto alignOffset{ static_cast<Dod::MemTypes::capacity_t>(MemUtils::getAlignOffset(initialSourceBegin, alignment)) };
		const auto alignedSourceBegin{ initialSourceBegin + alignOffset };

		const auto capacity{ static_cast<Dod::MemTypes::capacity_t>(source.dataEnd - source.dataBegin) };
		if ((source.dataBegin == nullptr) || (source.dataEnd == nullptr) || (header >= capacity) || (numOfBytes + alignOffset > capacity - header))
			return output;

		const auto totalBytes{ numOfBytes + alignOffset };

		output.dataBegin = alignedSourceBegin;
		output.dataEnd = source.dataBegin + header + totalBytes;

		header += totalBytes;

		return output;

	}

};
