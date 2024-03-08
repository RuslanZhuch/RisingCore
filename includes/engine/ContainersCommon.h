#include <type_traits>
#include <concepts>
#include <memory>

namespace Engine::ContainerUtils
{

	template <typename ContainerType>
	concept ContainerWithSize = requires(ContainerType t) {
		t.size;
	};

	template <typename ContainerType>
	concept ContainerWithCapacity = requires(ContainerType t) {
		t.cap;
	};

	template<typename T>
	concept IsCStyleArray = std::is_array_v<std::remove_reference_t<T>> &&
		!std::is_pointer_v<std::remove_reference_t<T>>;

	[[nodiscard]] constexpr auto getCapacity(ContainerWithCapacity auto&& vector)
	{
		return vector.cap;
	}

	[[nodiscard]] constexpr auto getNumOfElements(ContainerWithSize auto&& vector)
	{
		return vector.size;
	}


}