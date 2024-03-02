#include <cinttypes>

namespace Engine
{

	template <typename Type, int32_t capacity>
	struct StaticVector
	{
		using type_t = Type;
		constexpr static int32_t cap = capacity;
		int32_t size;
		Type data[capacity];
	};

}
