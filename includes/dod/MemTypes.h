#include <cstddef>
#include <cinttypes>

namespace Dod::MemTypes
{

	using data_t = std::byte;
	using dataPoint_t = data_t*;
	using dataConstPoint_t = const data_t*;
	using capacityEls_t = int32_t;
	using capacity_t = int64_t;
	using alignment_t = uintptr_t;

};
