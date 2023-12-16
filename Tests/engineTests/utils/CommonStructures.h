#pragma once

#include <dod/MemTypes.h>

struct MemorySpan
{
	Dod::MemTypes::dataPoint_t dataBegin{};
	Dod::MemTypes::dataPoint_t dataEnd{};
};

struct MemoryConstSpan
{
	Dod::MemTypes::dataConstPoint_t dataBegin{};
	Dod::MemTypes::dataConstPoint_t dataEnd{};
};
