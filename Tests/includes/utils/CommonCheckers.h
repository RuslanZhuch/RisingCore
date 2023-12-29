#pragma once

static void checkMemoryAligned(const auto* point, uintptr_t alignment)
{

	const auto address{ reinterpret_cast<std::uintptr_t>(point) };
	EXPECT_EQ(address % alignment, 0);

}
