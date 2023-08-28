#pragma once

#include <type_traits>

namespace Dod::CommonData
{

	struct Buffer {};
	struct Table {};

	template <typename T>
	concept CBuffer = std::is_base_of_v<Buffer, T>;

	template <typename T>
	concept CTable = std::is_base_of_v<Table, T>;

}
