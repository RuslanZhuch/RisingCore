#include <cinttypes>

namespace Engine
{

	template <typename TKey, typename TValue, int32_t capacity>
	struct StaticMap
	{
		using keyType_t = TKey;
		using valuesType_t = TValue;
		struct Data
		{
			TKey keys[capacity];
			TValue values[capacity];
		};

		constexpr static int32_t cap = capacity;
		int32_t size;
		Data data;
	};

}
